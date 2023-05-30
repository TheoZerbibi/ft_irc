#include "PrivMsgCommand.hpp"

PrivMsgCommand::PrivMsgCommand(void) :
_name("PRIVMSG")
{
	std::cout << "┣⮕ PrivMsgCommand." << std::endl;
	getCommandList()->insert(std::pair<std::string, Command*>(this->_name, this));
}

PrivMsgCommand::~PrivMsgCommand(void)
{}

void PrivMsgCommand::execute(void)
{
	std::cout << "[" << this->_name << "] : PrivMsgCommand executed !" << std::endl;
}


