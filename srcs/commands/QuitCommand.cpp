#include "QuitCommand.hpp"

QuitCommand::QuitCommand(void) :
_name("QUIT")
{
	std::cout << "┣⮕ QuitCommand." << std::endl;
}

QuitCommand::~QuitCommand(void)
{}

void QuitCommand::execute(int fds, Client *client)
{
	(void) fds;
	(void) client;
	std::cout << "[" << this->_name << "] : QuitCommand executed !" << std::endl;
}
