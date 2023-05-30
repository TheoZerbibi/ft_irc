#include "Command.hpp"

Command::Command(void) :
_name("Default")
{}

Command::~Command(void)
{}

void Command::execute(void)
{
	std::cout << this->_name << std::endl;
}

std::map<std::string, Command*> *getCommandList()
{
	static std::map<std::string, Command*> commandList;
	return (&commandList);
}
