#include "InviteCommand.hpp"

InviteCommand::InviteCommand(void) :
_name("INVITE")
{
	std::cout << "┣⮕ InviteCommand." << std::endl;
	getCommandList()->insert(std::pair<std::string, Command*>(this->_name, this));
}

InviteCommand::~InviteCommand(void)
{}

void InviteCommand::execute(void)
{
	std::cout << "[" << this->_name << "] : InviteCommand executed !" << std::endl;
}
