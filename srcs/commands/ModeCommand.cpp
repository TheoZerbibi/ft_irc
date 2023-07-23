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
	std::cout << "MODE : Applying mode" << std::endl;
	if (mode == 'i')
		chan->setInvit(modmode);
	else if (mode == 'o')
	{
		std::cout << "doing o mode --> user: " << *arg << " mode = " << modmode << std::endl;
		chan->setOper(*arg, modmode);
	}
	else if (mode == 't')
		chan->setTopicMode(modmode);
	else if (mode == 'k')
	{
		if (modmode == REMOVING)
			chan->setKey("");
		else
		{
			std::cout << "MODE +k : setting key to '" << *arg << "'" << std::endl;
			chan->setKey(*arg);
		}
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
			modmode = (*beg == '+' ? ADDING : REMOVING);
		else if (checkMode(*beg, modmode, user, chan, arg))
		{
			needParam = needArg(*beg, modmode);
			if (needParam)
			{
				std::cout << "MODE : this mode need argument :" << arg.at(0) << std::endl;
				this->appChannelMode(*beg, modmode, chan, &arg.at(0));
				value = arg.at(0);
				arg.erase(arg.begin());
				std::cout << "MODE : ended applying mode" << std::endl;
			}
			else
			{
				std::cout << "MODE : this mode don't need argument" << std::endl;
				this->appChannelMode(*beg, modmode, chan, NULL);
			}
			if (needParam)
			{
				std::cout << "MODE : Adding Reply" << std::endl;
				chan->sendToEveryone(user, RPL_MODEWITHARG(user_id(ircserv.getName(), user->getNickname(), user->getUsername()), chan->getName(), (modmode == ADDING ? '+' : '-') + *beg, value));
				std::cout << "MODE : Ended Adding Reply" << std::endl;
			}
			else
				chan->sendToEveryone(user, RPL_MODE(user_id(ircserv.getName(), user->getNickname(), user->getUsername()), chan->getName(), (modmode == ADDING ? '+' : '-') + *beg));
		}
		++beg;
	}
}

//	if (!user->isChannelOper(chan))
//		return (ircserv.addReply(Reply(fds, ERR_CHANOPRIVSNEEDED(ircserv.getName(), user->getNickname(), chan->getName()))));
//}

void	ModeCommand::executeChannelMode(int fds, User *user, std::vector<std::string> &args)
{
	Irc				&ircserv = Irc::getInstance();
	Channel 			*chan = ircserv.getChannel(args.at(0));
	std::string			modstr;

	if (!chan)
		return (ircserv.addReply(Reply(fds, ERR_NOSUCHCHANNEL(ircserv.getName(), user->getNickname(), args.at(0)))));
	std::cout << "MODE : Found the channel" << std::endl;
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

	std::cout << "MODE: Starting" << std::endl;
	if (args.empty())
		return (ircserv.addReply(Reply(fds, ERR_NEEDMOREPARAMS(ircserv.getName(), client->getNickname(), this->_name))));
	std::cout << "MODE: Enough param" << std::endl;
	if (args.at(0)[0] == '#')
	{
		std::cout << "MODE: Channel operation" << std::endl;
		executeChannelMode(fds, dynamic_cast<User *>(client), args);
	}
	else	
	{
		std::cout << "MODE: User operation (to be implemented soon)" << std::endl;
		executeUserMode(fds, dynamic_cast<User *>(client), args);
	}
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

int			ModeCommand::checkMode(char mode, bool modmode, User *user, Channel *chan, std::vector<std::string> &args)
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
		if (args.empty())
		{
			ircserv.addReply(Reply(user->getSockfd(), ERR_NEEDMOREPARAMS(ircserv.getName(), user->getNickname(), this->_name)));
			return (false);
		}
		if (mode == 'o')
		{
			if (!ircserv.getUserByNick(args.at(0)))
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

//	void	applyUserMode(int fds, User *user, Client *target, std::string &modstr, std::vector<std::string> *modvalue)
//	{
//		Irc 	&ircserv= Irc::getInstance();
//		int	mod_mode = ADDING;
//		(void)modvalue;
//	
//		std::vector<std::string>::iterator  beg = modstr.begin();
//		std::vector<std::string>::iterator  end = modstr.end();
//		while (beg != end)
//		{
//			if (*beg == '+')
//				mod_mode = ADDING;
//			else if (*beg == '-')
//				mod_mode = REMOVING;
//			else if (*beg == 'i')
//					user.setInvis(mod_mode);
//			else if (*beg == 'o')
//			{
//				if	(user->isOper())
//					target->setInvis(mod_mode);
//			}
//			else
//			{
//			ircserv.addReply(Reply(fds, ERR_UMODEUNKNOWNFLAG(ircserv.getName(), user->getNickname())));
//			}
//			++beg;
//		}
//	}

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
	//	args.erase(args.begin());
	//	modestr = args.at(0);
	//	std::cout << "modestr = " << modestr << std::endl;
	//	args.erase(args.begin());
}

//	void ModeCommand::changeUserMode(int fds, User *user, Client *target, std::vector<std::string> &args)
//	{
//		Irc				&ircserv = Irc::getInstance();
//	
//		if (target != user)
//		{
//			return (ircserv.addReply(Reply(fds, ERR_USERSDONTMATCH(ircserv.getName(), user->getNickname()))));
//		}
//	}


