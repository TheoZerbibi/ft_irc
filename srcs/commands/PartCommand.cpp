#include "PartCommand.hpp"

PartCommand::PartCommand(void) :
_name("PART")
{
	std::cout << "┣⮕ PartCommand." << std::endl;
	getCommandList()->insert(std::pair<std::string, Command*>(this->_name, this));
}

PartCommand::~PartCommand(void)
{}

void PartCommand::execute(void)
{
	std::cout << "[" << this->_name << "] : PartCommand executed !" << std::endl;
}
