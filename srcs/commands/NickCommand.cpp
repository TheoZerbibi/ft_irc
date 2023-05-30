#include "NickCommand.hpp"

NickCommand::NickCommand(void) :
_name("NICK")
{
	std::cout << "┣⮕ NickCommand." << std::endl;
}

NickCommand::~NickCommand(void)
{}

void NickCommand::execute(int fds, Client *client)
{
	(void) fds;
	(void) client;
	std::cout << "[" << this->_name << "] : NickCommand executed !" << std::endl;
}
