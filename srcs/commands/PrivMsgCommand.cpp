#include "PrivMsgCommand.hpp"

PrivMsgCommand::PrivMsgCommand(void) :
_name("PRIVMSG")
{
	std::cout << "┣⮕ PrivMsgCommand." << std::endl;
}

PrivMsgCommand::~PrivMsgCommand(void)
{}

void PrivMsgCommand::execute(int fds, Client *client)
{
	(void) fds;
	(void) client;
	std::cout << "[" << this->_name << "] : PrivMsgCommand executed !" << std::endl;
}


