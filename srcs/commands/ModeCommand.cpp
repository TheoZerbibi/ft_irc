#include "ModeCommand.hpp"

ModeCommand::ModeCommand(void) :
_name("MODE")
{
	std::cout << "┣⮕ ModeCommand." << std::endl;
}

ModeCommand::~ModeCommand(void)
{}

bool
	ModeCommand::cantExecute(Client *client)
{
	return (client->isRegistered());
}

void	ModeCommand::printChannelMode(int fds, User *user, Channel *chan)
{
	Irc				&ircserv = Irc::getInstance();
	std::string			modestring = "";
	std::string			mode_value = "";
	std::stringstream		ss;

	const std::string			key_value = chan->getKey();
	const int				user_limit = chan->getLimit();
	ss << user_limit;

	if ((key_value != ""))
	{
		modestring += "k";
		if (user->isChannelOper(chan))
			mode_value += key_value;
		else
			mode_value += "<key>";
	}
	if (chan->topicIsRestricted())
		modestring += "t";
	if ((user_limit))
	{
		modestring += "l";
		if (mode_value != "")
			mode_value += " ";
		mode_value += ss.str();
	}
	if (chan->isInvit())
	{
		modestring += "i";
	}
	return (ircserv.addReply(Reply(fds, RPL_CHANNELMODIS(ircserv.getName(), user->getNickname(), chan->getName(), "+" + modestring, mode_value))));
}

void	ModeCommand::appChannelMode(char mode, int modmode, Channel *chan, std::string *arg)
{
	if (mode == 'i')
		chan->setInvit(modmode);
	else if (mode == 'o')
		chan->setOper(*arg, modmode);
	else if (mode == 't')
		chan->setTopicMode(modmode);
	else if (mode == 'k')
	{
		if (modmode == REMOVING)
			chan->setKey("");
		else
			chan->setKey(*arg);
	}
	else if (mode == 'l')
	{
		if (modmode == REMOVING)
			chan->setMaxUser(0);
		else
		{
			if (std::atoi(arg->c_str()) > 0)
				chan->setMaxUser(std::atoi(arg->c_str()));
		}
	}
}

int	ModeCommand::checkOperMode(int fds, User *user, std::string &target)
{
	Irc	&ircserv = Irc::getInstance();

	if (!ircserv.getUserByNick(target))
	{
		ircserv.addReply(Reply(fds, ERR_NOSUCHNICK(ircserv.getName(), user->getNickname())));
		return (0);
	}
	return (1);
}

void	ModeCommand::applyChannelMode(User *user, Channel *chan, std::string &modstr, std::vector<std::string> &arg)
{
	std::string::iterator	beg = modstr.begin();
	std::string::iterator	end = modstr.end();
	std::string		value;
	int						modmode = ADDING;
	int						needParam = false;
	Irc						&ircserv = Irc::getInstance();

	while (beg != end)
	{
		if (*beg == '+' || *beg == '-')
		{
			modmode = (*beg == '+' ? ADDING : REMOVING);
			beg++;
		}
		if ((needParam = needArg(*beg, modmode)))
		{
			value = *(arg.begin());
			arg.erase(arg.begin());
		}
		else
			value = "";
		if (checkMode(*beg, modmode, user, chan, value))
		{
			if (needParam)
			{
				this->appChannelMode(*beg, modmode, chan, &value);
				chan->sendToEveryone(user, RPL_MODEWITHARG(user_id(ircserv.getName(), user->getNickname(), user->getUsername()), chan->getName(), (modmode == ADDING ? '+' : '-') + *beg, value));
			}
			else
			{
				this->appChannelMode(*beg, modmode, chan, NULL);
				chan->sendToEveryone(user, RPL_MODE(user_id(ircserv.getName(), user->getNickname(), user->getUsername()), chan->getName(), (modmode == ADDING ? '+' : '-') + *beg));
			}
		}
		++beg;
	}
}

void	ModeCommand::executeChannelMode(int fds, User *user, std::vector<std::string> &args)
{
	Irc				&ircserv = Irc::getInstance();
	Channel 			*chan = ircserv.getChannel(args.at(0));
	std::string			modstr;

	if (!chan)
		return (ircserv.addReply(Reply(fds, ERR_NOSUCHCHANNEL(ircserv.getName(), user->getNickname(), args.at(0)))));
	args.erase(args.begin());
	if (args.size() == 0)
		return (printChannelMode(fds, user, chan));
	modstr = args.at(0);
	args.erase(args.begin());
	this->applyChannelMode(user, chan, modstr, args);
}

void ModeCommand::execute(int fds, Client *client)
{
	std::string			cmd = client->getCmds().front();
	std::vector<std::string>	args = splitArguments(cmd);
	Irc				&ircserv = Irc::getInstance();

	if (args.empty())
		return (ircserv.addReply(Reply(fds, ERR_NEEDMOREPARAMS(ircserv.getName(), client->getNickname(), this->_name))));
	if (args.at(0)[0] == '#')
		executeChannelMode(fds, dynamic_cast<User *>(client), args);
	else
		executeUserMode(fds, dynamic_cast<User *>(client), args);
	std::cout << "[" << this->_name << "] : ModeCommand executed !" << std::endl;
}

bool			ModeCommand::needArg(char mode, bool modmode)
{
	std::string	modes = MODES_WARG;
	size_t	pos = modes.find(mode);
	if (mode == 'o' || (modmode == ADDING && (pos != std::string::npos)))
	{
		return (true);
	}
	return (false);
}

int			ModeCommand::checkMode(char mode, bool modmode, User *user, Channel *chan, std::string arg)
{
	Irc			&ircserv = Irc::getInstance();
	std::string		modes = MODES;
	size_t			pos = modes.find(mode);


	if (!chan->getOper(user->getNickname()))
	{
		ircserv.addReply(Reply(user->getSockfd(), ERR_CHANOPRIVSNEEDED(ircserv.getName(), user->getNickname(), chan->getName())));
		return (false);
	}
	if (pos == std::string::npos)
	{
		ircserv.addReply(Reply(user->getSockfd(), ERR_MODEUNKNOWN(ircserv.getName(), user->getNickname(), mode)));
		return (false);
	}
	std::string		modes_warg = MODES_WARG;
	if (needArg(mode, modmode))
	{
		if (arg.empty())
		{
			ircserv.addReply(Reply(user->getSockfd(), ERR_NEEDMOREPARAMS(ircserv.getName(), user->getNickname(), this->_name)));
			return (false);
		}
		if (mode == 'o')
		{
			if (!ircserv.getUserByNick(arg))
			{
				ircserv.addReply(Reply(user->getSockfd(), ERR_NOSUCHNICK(ircserv.getName(), user->getNickname())));
				return (false);
			}
		}
	}
	return (true);
}


void	ModeCommand::printUserMode(int fds, User *user, std::string nickname)
{
	Irc				&ircserv = Irc::getInstance();
	std::string			modes = "";

	if (nickname != user->getNickname())
		return (ircserv.addReply(Reply(fds, ERR_USERSDONTMATCHVIEW(ircserv.getName(), nickname))));
	if (user->isInvis())
		modes += "i";
	if (user->isOper())
		modes += "o";
	ircserv.addReply(Reply(fds, RPL_UMODEIS(ircserv.getName(), nickname, modes)));
}

void	ModeCommand::executeUserMode(int fds, User *user, std::vector<std::string> &args)
{
	Irc 	&ircserv= Irc::getInstance();

	const User		*target = ircserv.getUserByNick(args.at(0));
	std::string		modestr;

	if (!target)
		return (ircserv.addReply(Reply(fds, ERR_NOSUCHNICK(ircserv.getName(), user->getNickname()))));
	args.erase(args.begin());
	if (args.size() == 0)
	{
		return (printUserMode(fds, user, target->getNickname()));
	}
	if (target != user)
		return (ircserv.addReply(Reply(fds, ERR_USERSDONTMATCH(ircserv.getName(), user->getNickname()))));
}


