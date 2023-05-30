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
	(void) fds;
	(void) client;
	std::cout << "[" << this->_name << "] : UserCommand executed !" << std::endl;
}
