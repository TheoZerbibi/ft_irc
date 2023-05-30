#include "Command.hpp"

Command::Command(void) :
_name("Default")
{}

Command::~Command(void)
{}

void Command::execute(int fds, Client *client)
{
	(void) fds;
	(void) client;
	std::cout << this->_name << std::endl;
}
