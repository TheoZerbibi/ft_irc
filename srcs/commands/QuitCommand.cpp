#include "QuitCommand.hpp"

QuitCommand::QuitCommand(void) :
_name("QUIT")
{
	std::cout << "┣⮕ QuitCommand." << std::endl;
}

QuitCommand::~QuitCommand(void)
{}

// Actuellement, si tu rejoint un chan et que tu /quit, ca segfault

void QuitCommand::execute(int fds, Client *client)
{
	Irc		&ircserv = Irc::getInstance();

	if (client) {
		ircserv.removeClient(fds);
		// client->quitAllChannel();
		delete client;
	}
	std::cout << "QuitCommand::execute(" << fds << ")" << std::endl;
}
