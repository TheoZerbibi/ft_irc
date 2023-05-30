#include "TopicCommand.hpp"

TopicCommand::TopicCommand(void) :
_name("TOPIC")
{
	std::cout << "┣⮕ TopicCommand." << std::endl;
	getCommandList()->insert(std::pair<std::string, Command*>(this->_name, this));
}

TopicCommand::~TopicCommand(void)
{}

void TopicCommand::execute(void)
{
	std::cout << "[" << this->_name << "] : TopicCommand executed !" << std::endl;
}
