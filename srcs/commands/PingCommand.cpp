#include "PingCommand.hpp"
#include <bits/stdc++.h>

PingCommand::PingCommand(void) :
_name("PING")
{
	std::cout << "┣⮕ PingCommand." << std::endl;
}

PingCommand::~PingCommand(void)
{}

bool
	PingCommand::cantExecute(Client *client)
{
	return (client->isRegistered());
}

void PingCommand::execute(int fds, Client *client)
{
	std::string	cmd =			client->getCmds().front();
	std::vector<std::string>	tokenList = this->splitArguments(cmd);
	std::stringstream 			ss;
	Irc							&ircserv =	Irc::getInstance();

	std::string const			serverName = ircserv.getName();
	std::string const			nick = client->getNick();
	std::string const			user = client->getUser();

	for (std::vector<std::string>::iterator it = tokenList.begin(); it != tokenList.end(); ++it)
		ss << *it << " ";

	ircserv.addReply(Reply(fds, RPL_PONG(user_id(serverName, nick, user), ss.str())));
	std::cout << "PingCommand::execute(" << fds << ", " << ss.str() << ")" << std::endl;
}
