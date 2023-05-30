#include "UserCommand.hpp"

UserCommand::UserCommand(void) :
_name("USER")
{
	std::cout << "┗⮕ UserCommand." << std::endl;
	getCommandList()->insert(std::pair<std::string, Command*>(this->_name, this));
}

UserCommand::~UserCommand(void)
{}

void UserCommand::execute(void)
{
	std::cout << "[" << this->_name << "] : UserCommand executed !" << std::endl;
}
