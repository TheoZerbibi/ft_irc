#include "CapCommand.hpp"

CapCommand::CapCommand(void) :
_name("CAP")
{
	std::cout << "┣⮕ CapCommand." << std::endl;
}

CapCommand::~CapCommand(void)
{}

bool
	CapCommand::cantExecute(Client *client)
{
	(void) client;
	return (true);
}

void CapCommand::execute(int fds, Client *client)
{
	(void) fds;
	(void) client;
	std::cout << "[" << this->_name << "] : CapCommand executed !" << std::endl;
}
