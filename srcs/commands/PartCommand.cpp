#include "PartCommand.hpp"

PartCommand::PartCommand(void) :
_name("PART")
{
	std::cout << "┣⮕ PartCommand." << std::endl;
}

PartCommand::~PartCommand(void)
{}

void PartCommand::execute(int fds, Client *client)
{
	(void) fds;
	(void) client;
	Irc		&ircserv = Irc::getInstance();
	User	*user = ircserv.getUserByNick(client->getNick());
	user->quitAllChannel();
	std::cout << "[" << this->_name << "] : PartCommand executed !" << std::endl;
}
