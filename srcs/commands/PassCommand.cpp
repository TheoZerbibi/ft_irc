#include "PassCommand.hpp"

PassCommand::PassCommand(void) :
_name("PASS")
{
	std::cout << "┣⮕ PassCommand." << std::endl;
}

PassCommand::~PassCommand(void)
{}

void PassCommand::execute(int fds, Client *client)
{
	std::string	cmd = client->getCmds().front();
	std::string	pass = cmd.substr(cmd.find(" ") + 1);
	Irc	&ircserv = Irc::getInstance();

	if (pass.empty() || ircserv.getPass() != pass)
	{
		std::cout << "PassCommand::execute(" << fds << ", " <<  "Invalid pass )" << std::endl;
		return ;
	} else if (ircserv.getPass() == pass)
		std::cout << "PassCommand::execute(" << fds << ", " << pass << ")" << std::endl;
}
