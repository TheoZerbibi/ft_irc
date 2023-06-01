#include "ModeCommand.hpp"

ModeCommand::ModeCommand(void) :
_name("MODE")
{
	std::cout << "┣⮕ ModeCommand." << std::endl;
}

ModeCommand::~ModeCommand(void)
{}

void ModeCommand::execute(int fds, Client *client)
{
	(void) fds;
	(void) client;
	std::cout << "[" << this->_name << "] : ModeCommand executed !" << std::endl;
}
