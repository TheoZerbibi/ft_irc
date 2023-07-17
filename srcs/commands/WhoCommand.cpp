#include "WhoCommand.hpp"

WhoCommand::WhoCommand(void) :
_name("WHO")
{
	std::cout << "┗⮕ WhoCommand." << std::endl;
}

WhoCommand::~WhoCommand(void)
{}

bool
	WhoCommand::cantExecute(Client *client)
{
	(void) client;
	return (true);
}

void WhoCommand::execute(int fds, Client *client)
{
	(void) fds;
	(void) client;
	std::cout << "[" << this->_name << "] : WhoCommand executed !" << std::endl;
}
