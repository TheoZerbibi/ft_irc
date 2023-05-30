#include "NickCommand.hpp"

NickCommand::NickCommand(void) :
_name("NICK")
{
	std::cout << "┣⮕ NickCommand." << std::endl;
	getCommandList()->insert(std::pair<std::string, Command*>(this->_name, this));
}

NickCommand::~NickCommand(void)
{}

void NickCommand::execute(void)
{
	std::cout << "[" << this->_name << "] : NickCommand executed !" << std::endl;
}
