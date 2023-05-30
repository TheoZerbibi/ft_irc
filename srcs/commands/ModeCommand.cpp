#include "ModeCommand.hpp"

ModeCommand::ModeCommand(void) :
_name("MODE")
{
	std::cout << "┣⮕ ModeCommand." << std::endl;
	getCommandList()->insert(std::pair<std::string, Command*>(this->_name, this));
}

ModeCommand::~ModeCommand(void)
{}

void ModeCommand::execute(void)
{
	std::cout << "[" << this->_name << "] : ModeCommand executed !" << std::endl;
}
