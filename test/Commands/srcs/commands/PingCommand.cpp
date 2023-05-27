#include "PingCommand.hpp"

PingCommand::PingCommand(void) :
_name("Ping")
{}

PingCommand::~PingCommand(void)
{}

void PingCommand::execute(void)
{
	std::cout << "[" << this->_name << "] : Pong !" << std::endl;
}
