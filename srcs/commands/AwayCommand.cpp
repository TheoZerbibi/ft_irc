#include "AwayCommand.hpp"

AwayCommand::AwayCommand(void) :
_name("AWAY")
{
	std::cout << "┣⮕ AwayCommand." << std::endl;
}

AwayCommand::~AwayCommand(void)
{}

void AwayCommand::execute(int fds, Client *client)
{
	(void) fds;
	(void) client;
	std::cout << "[" << this->_name << "] : AwayCommand executed !" << std::endl;
}
