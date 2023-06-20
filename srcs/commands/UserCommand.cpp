#include "UserCommand.hpp"


UserCommand::UserCommand(void) :
_name("USER")
{
	std::cout << "┗⮕ UserCommand." << std::endl;
}

UserCommand::~UserCommand(void)
{}

void UserCommand::execute(int fds, Client *client)
{
	std::string	cmd = client->getCmds().front();
	std::vector<std::string> arguments = splitArguments(cmd);
	Irc	&ircserv = Irc::getInstance();

	if (client->isRegistered())
		ircserv.addReply(Reply(fds, ERR_ALREADYREGISTERED(ircserv.getName(), client->getNick())));
	else if (arguments.size() != 5)
		ircserv.addReply(Reply(fds, ERR_NEEDMOREPARAMS(ircserv.getName(), client->getNick(), this->_name)));
	else {
		client->setUser(arguments[1]);
		client->setHost(arguments[3]);
		client->setRealname(arguments[4]);
		std::cout << "UserCommand::execute(" << fds << ", " << arguments.at(1) << ", " << arguments.at(2) << ", " << arguments.at(3) << ", " << arguments.at(4) << ")" << std::endl;
		client->readyToRegister();
	}
}
