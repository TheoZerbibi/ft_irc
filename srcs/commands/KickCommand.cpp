#include "KickCommand.hpp"

KickCommand::KickCommand(void) :
_name("KICK")
{
	std::cout << "┣⮕ KickCommand." << std::endl;
}

KickCommand::~KickCommand(void)
{}

bool
	KickCommand::cantExecute(Client *client)
{
	return (client->isRegistered());
}

void KickCommand::execute(int fds, Client *client)
{
	(void) fds;
	(void) client;
	std::cout << "[" << this->_name << "] : KickCommand executed !" << std::endl;
}
