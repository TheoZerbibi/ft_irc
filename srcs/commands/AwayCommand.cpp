#include "AwayCommand.hpp"

AwayCommand::AwayCommand(void) :
_name("AWAY")
{
	std::cout << "┣⮕ AwayCommand." << std::endl;
	// getCommandList()->insert(std::pair<std::string, Command*>(this->_name, this));
}

AwayCommand::~AwayCommand(void)
{}

void AwayCommand::execute(int fds, Client *client)
{
	(void) fds;
	(void) client;
	std::cout << "[" << this->_name << "] : AwayCommand executed !" << std::endl;
}
