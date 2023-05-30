#include "TopicCommand.hpp"

TopicCommand::TopicCommand(void) :
_name("TOPIC")
{
	std::cout << "┣⮕ TopicCommand." << std::endl;
}

TopicCommand::~TopicCommand(void)
{}

void TopicCommand::execute(int fds, Client *client)
{
	(void) fds;
	(void) client;
	std::cout << "[" << this->_name << "] : TopicCommand executed !" << std::endl;
}
