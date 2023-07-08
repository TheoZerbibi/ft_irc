#include "QuitCommand.hpp"

QuitCommand::QuitCommand(void) :
_name("QUIT")
{
	std::cout << "┣⮕ QuitCommand." << std::endl;
}

QuitCommand::~QuitCommand(void)
{}

bool
	QuitCommand::cantExecute(Client *client)
{
	return (client->isRegistered());
}

void QuitCommand::execute(int fds, Client *client)
{
	Irc		&ircserv = Irc::getInstance();
	std::string	cmd = client->getCmds().front();
	std::vector<std::string>	args = splitArguments(cmd);

	ircserv.removeClient(fds, args[0]);

	std::cout << "QuitCommand::execute(" << fds << ")" << std::endl;
}
