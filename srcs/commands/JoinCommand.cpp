#include "JoinCommand.hpp"

JoinCommand::JoinCommand(void) :
_name("JOIN")
{
	std::cout << "┣⮕ JoinCommand." << std::endl;
	getCommandList()->insert(std::pair<std::string, Command*>(this->_name, this));
}

JoinCommand::~JoinCommand(void)
{}

void JoinCommand::execute(void)
{
	std::cout << "[" << this->_name << "] : JoinCommand executed !" << std::endl;
}
