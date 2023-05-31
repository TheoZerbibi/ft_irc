#include "NickCommand.hpp"

NickCommand::NickCommand(void) :
_name("NICK")
{
	std::cout << "┣⮕ NickCommand." << std::endl;
}

NickCommand::~NickCommand(void)
{}

void NickCommand::execute(int fds, Client *client)
{
	(void)fds;
	std::string	cmd = client->getCmds().front();
	std::string	nick = cmd.substr(cmd.find(" ") + 1);

	if (nick.empty())
	{
		std::cout << "NickCommand::execute(" << fds << ", " <<  "Invalid nick )" << std::endl;
		return ;
	}
	client->setNick(nick);
	std::cout << "NickCommand::execute(" << fds << ", " << client->getNick() << ")" << std::endl;
}
