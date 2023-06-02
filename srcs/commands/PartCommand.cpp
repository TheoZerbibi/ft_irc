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
	std::cout << "[" << this->_name << "] : PartCommand executed !" << std::endl;
}
