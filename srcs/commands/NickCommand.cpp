#include "NickCommand.hpp"

NickCommand::NickCommand(void) :
_name("NICK")
{
	std::cout << "┣⮕ NickCommand." << std::endl;
}

NickCommand::~NickCommand(void)
{}

void NickCommand::execute(int fds, Client *client)
{
	std::string	cmd = client->getCmds().front();
	size_t		pos = cmd.find(" ");
	std::string	nick = cmd.substr(cmd.find(" ") + 1);
	Irc		&ircserv = Irc::getInstance();

	std::cout << "POS = " << pos << std::endl;
	std::cout << "NICKNAME = " << nick << std::endl;
	if (nick.empty())
	{
		ircserv.addReply(Reply(fds, ERR_NEEDMOREPARAMS(ircserv.getName(), client->getNick(), this->_name)));
		return ;
	}
	if (ircserv.getUserByNick(nick))
	{
		ircserv.addReply(Reply(fds, ERR_NICKNAMEINUSE(ircserv.getName(), client->getNick(), nick)));
		return ;
	}
	client->setNick(nick);
	std::cout << "NickCommand::execute(" << fds << ", " << client->getNick() << ")" << std::endl;
}
