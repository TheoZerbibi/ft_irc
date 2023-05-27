#include "TestCommand.hpp"

TestCommand::TestCommand(void) :
_name("Test")
{}

TestCommand::~TestCommand(void)
{}

void TestCommand::execute(void)
{
	std::cout << "[" << this->_name << "] : TestCommand executed !" << std::endl;
}
