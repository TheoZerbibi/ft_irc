#include "CapCommand.hpp"

CapCommand::CapCommand(void) :
_name("CAP")
{
	std::cout << "┣⮕ CapCommand." << std::endl;
	getCommandList()->insert(std::pair<std::string, Command*>(this->_name, this));
}

CapCommand::~CapCommand(void)
{}

void CapCommand::execute(void)
{
	std::cout << "[" << this->_name << "] : CapCommand executed !" << std::endl;
}
