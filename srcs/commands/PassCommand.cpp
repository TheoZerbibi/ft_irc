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

	if (client->isRegistered())
		ircserv.addReply(Reply(fds, ERR_ALREADYREGISTERED(client->getNick())));
	else if (pass.empty())
		ircserv.addReply(Reply(fds, ERR_NEEDMOREPARAMS(client->getNick(), "PASS")));
	else if (ircserv.getPass() == pass)
		client->setAuth(true);
	else
		ircserv.addReply(Reply(fds, ERR_PASSWDMISMATCH(client->getNick())));
	std::cout << "PassCommand::execute(" << fds << ", " << pass << ")" << std::endl;
}
