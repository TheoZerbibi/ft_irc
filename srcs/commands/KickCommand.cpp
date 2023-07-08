#include "KickCommand.hpp"

KickCommand::KickCommand(void) :
_name("KICK")
{
	std::cout << "┣⮕ KickCommand." << std::endl;
}

KickCommand::~KickCommand(void)
{}

/**
** -> KICK #test1,#test2 to,tba :Adios Bahamas
** -> KICK #test to :You should not stay !
** -> KICK #test to,tba :Your friend is in another channel!
** -> KICK #test to
**/

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
