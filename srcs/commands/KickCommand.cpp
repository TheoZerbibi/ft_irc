#include "KickCommand.hpp"

KickCommand::KickCommand(void) :
_name("KICK")
{
	std::cout << "┣⮕ KickCommand." << std::endl;
	getCommandList()->insert(std::pair<std::string, Command*>(this->_name, this));
}

KickCommand::~KickCommand(void)
{}

void KickCommand::execute(void)
{
	std::cout << "[" << this->_name << "] : KickCommand executed !" << std::endl;
}
