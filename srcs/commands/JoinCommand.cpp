#include "JoinCommand.hpp"

JoinCommand::JoinCommand(void) :
_name("JOIN")
{
	std::cout << "┣⮕ JoinCommand." << std::endl;
}

JoinCommand::~JoinCommand(void)
{}

void JoinCommand::execute(int fds, Client *client)
{
	(void) fds;
	(void) client;
	std::cout << "[" << this->_name << "] : JoinCommand executed !" << std::endl;
}
