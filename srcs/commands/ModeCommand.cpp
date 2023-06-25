#include "ModeCommand.hpp"

ModeCommand::ModeCommand(void) :
_name("MODE")
{
	std::cout << "┣⮕ ModeCommand." << std::endl;
}

ModeCommand::~ModeCommand(void)
{}

void	ModeCommand::printUserMode(int fds, User *user, std::string nickname)
{
	Irc				&ircserv = Irc::getInstance();
	std::string			modes = "";

	if (nickname != user->getNick())
	{
		return (ircserv.addReply(Reply(fds, ERR_USERSDONTMATCHVIEW(ircserv.getName(), nickname))));
	}
	if (user->isInvis())
		modes += "i";
	if (user->isOper())
		modes += "o";
	ircserv.addReply(Reply(fds, RPL_UMODEIS(ircserv.getName(), nickname, modes)));
}


void ModeCommand::executeUserMode(int fds, User *user, std::vector<std::string> &args)
{
	Irc 	&ircserv= Irc::getInstance();


	Client		*target = NULL;//ircserv.getUserByNick();
	if (!target)
		return (ircserv.addReply(Reply(fds, ERR_NOSUCHNICK(ircserv.getName(), user->getNick()))));
	args.erase(args.begin());
	if (args.size() == 0)
		return (printUserMode(fds, user, target->getNick()));
//	if (target != user)
//		return (ircserv.addReply(Reply(fds, ERR_USERSDONTMATCH(ircserv.getName(), user->getNick()))));
}

//	void ModeCommand::changeUserMode(int fds, User *user, Client *target, std::vector<std::string> &args)
//	{
//		Irc				&ircserv = Irc::getInstance();
//	
//		if (target != user)
//		{
//			return (ircserv.addReply(Reply(fds, ERR_USERSDONTMATCH(ircserv.getName(), user->getNick()))));
//		}
//	}
	

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
	{
		modestring += "t";
	}
	if ((user_limit))
	{
		modestring += "l";
		if (mode_value != "")
			mode_value += " ";
		mode_value += ss.str();
	}
	return (ircserv.addReply(Reply(fds, RPL_CHANNELMODIS(ircserv.getName(), user->getNick(), chan->getName(), modestring, mode_value))));
}

void	ModeCommand::executeChannelMode(int fds, User *user, std::vector<std::string> &args)
{
	Irc				&ircserv = Irc::getInstance();
	Channel 			*chan = ircserv.getChannelByName(args.at(0));
	//	int				value_i = 1;
	//	int				mode_modifier = ADDING;


	if (!chan)
		return (ircserv.addReply(Reply(fds, ERR_NOSUCHCHANNEL(ircserv.getName(), user->getNick(), args.at(0)))));
	std::cout << "MODE : Found the channel" << std::endl;
	args.erase(args.begin());
	if (args.size() == 0)
		return (printChannelMode(fds, user, chan));
	if (!user->isChannelOper(chan))
		return (ircserv.addReply(Reply(fds, ERR_CHANOPRIVSNEEDED(ircserv.getName(), user->getNick(), chan->getName()))));
}

void ModeCommand::execute(int fds, Client *client)
{
	std::string			cmd = client->getCmds().front();
	std::vector<std::string>	args = splitArguments(cmd);
	Irc				&ircserv = Irc::getInstance();

	std::cout << "MODE: Starting" << std::endl;
	if (args.size() == 1)
		return (ircserv.addReply(Reply(fds, ERR_NEEDMOREPARAMS(ircserv.getName(), client->getNick(), this->_name))));
	args.erase(args.begin());
	std::cout << "MODE: Enough param" << std::endl;
	if (args.at(0)[0] == '#')
	{
		std::cout << "MODE: Channel operation" << std::endl;
		executeChannelMode(fds, dynamic_cast<User *>(client), args);
	}
	else	
	{
		std::cout << "MODE: User operation" << std::endl;
		executeUserMode(fds, dynamic_cast<User *>(client), args);
	}
	std::cout << "[" << this->_name << "] : ModeCommand executed !" << std::endl;
}
