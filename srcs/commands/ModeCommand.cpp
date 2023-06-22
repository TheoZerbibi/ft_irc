#include "ModeCommand.hpp"

ModeCommand::ModeCommand(void) :
_name("MODE")
{
	std::cout << "┣⮕ ModeCommand." << std::endl;
}

ModeCommand::~ModeCommand(void)
{}


//	void ModeCommand::executeUserMode(int fds, Client *user, std::vector<std::string> &args)
//	{
//		Client *target;
//	
//	//	if (target != user.
//	//	if (user.getNick() != args.at(1))
//	//		return ();
//	}
//	
//	void ModeCommand::changeUserMode(int fds, Client *user, Client *target, std::vector<std::string> &args)
//	{
//		Irc				&ircserv = Irc::getInstance();
//	
//		if (target != user)
//		{
//			return (ircserv.addReply(Reply(fds, ERR_USERSDONTMATCH(ircserv.getName(), user->getNick()))));
//		}
//	}
//	
//	void	ModeCommand::printChannelMode(int fds, Client *user, Channel *chan)
//	{
//		Irc				&ircserv = Irc::getInstance();
//	}
//	
//	void	ModeCommand::executeChannelMode(int fds, Client *user std::vector<std::string> &args)
//	{
//		Channel 			*chan = Irc::getChannel(channame);
//		if (!chan)
//			return (ircserv.addReply(Reply(fds, ERR_USERSDONTMATCH(ircserv.getName(), user->getNick()))));
//	}
//	
//	void	ModeCommand::printUserMode(int fds, Client *user, std::string nickname)
//	{
//		Irc				&ircserv = Irc::getInstance();
//		std::string			modes = "";
//	
//		if (nickname != user.getNick())
//		{
//			return (ircserv.addReply(Reply(fds, ERR_USERSDONTMATCHVIEW(ircserv.getName(), nickname))));
//		}
//		if (user.isInvis())
//			modes += "i";
//		if (user.isOper())
//			modes += "o";
//		ircserv.addReply(Reply(fds, RPL_UMODEIS(ircserv.getName(), nickname, modes)));
//	}

void ModeCommand::execute(int fds, Client *client)
{
	(void) fds;
	(void) client;
	std::string			cmd = client->getCmds().front();
	std::vector<std::string>	args = splitArguments(cmd);
	Irc				&ircserv = Irc::getInstance();

	if (args.size() == 1)
	{
		return (ircserv.addReply(Reply(fds, ERR_NEEDMOREPARAMS(ircserv.getName(), client->getNick(), this->_name))));
	}
	
//	if (args.at(1)[0] == '#')
//	{
//		
//	}
//	else	


	std::cout << "[" << this->_name << "] : ModeCommand executed !" << std::endl;
}
