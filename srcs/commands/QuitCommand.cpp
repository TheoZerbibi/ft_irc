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

// Actuellement, si tu rejoint un chan et que tu /quit, ca segfault
void QuitCommand::execute(int fds, Client *client)
{
	Irc		&ircserv = Irc::getInstance();
	std::string	cmd = client->getCmds().front();
	std::vector<std::string>	args = splitArguments(cmd);
	
	std::cout << "ARGS : " << args.size() << std::endl;

	ircserv.removeClient(fds);
	std::cout << "QuitCommand::execute(" << fds << ")" << std::endl;
}
