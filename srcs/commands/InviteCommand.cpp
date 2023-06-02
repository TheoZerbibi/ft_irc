#include "InviteCommand.hpp"

InviteCommand::InviteCommand(void) :
_name("INVITE")
{
	std::cout << "┣⮕ InviteCommand." << std::endl;
}

InviteCommand::~InviteCommand(void)
{}

void InviteCommand::execute(int fds, Client *client)
{
	(void) fds;
	(void) client;
	std::cout << "[" << this->_name << "] : InviteCommand executed !" << std::endl;
}
