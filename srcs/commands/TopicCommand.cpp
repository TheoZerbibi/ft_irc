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
	(void)fds;
	std::string			cmd = client->getCmds().front();
	std::vector<std::string>	args;
}
