#include "PassCommand.hpp"

PassCommand::PassCommand(void) :
_name("PASS")
{
	std::cout << "┣⮕ PassCommand." << std::endl;
}

PassCommand::~PassCommand(void)
{}


bool
	PassCommand::cantExecute(Client *client)
{
	Irc	 &ircserv = Irc::getInstance();

	if (client->isRegistered()) {
		ircserv.addReply(Reply(client->getSockfd(), ERR_ALREADYREGISTERED(ircserv.getName(), client->getNickname())));
		return (false);
	}
	return (true);
}
void PassCommand::execute(int fds, Client *client)
{
	std::string			cmd = client->getCmds().front();
	std::vector<std::string> 	args = this->splitArguments(cmd);
	Irc				&ircserv = Irc::getInstance();
	std::string			pass;

	if (args.empty())
		return (ircserv.addReply(Reply(fds, ERR_NEEDMOREPARAMS(ircserv.getName(), client->getNickname(), this->_name))));
	if (client->isRegistered())
		return (ircserv.addReply(Reply(fds, ERR_ALREADYREGISTERED(ircserv.getName(), client->getNickname()))));


	pass = args.at(0);
	if (ircserv.getPass() != pass)
		return (ircserv.addReply(Reply(fds, ERR_PASSWDMISMATCH(ircserv.getName(), client->getNickname()))));

	client->setAuth(true);
	if (!client->isRegistered())
		ircserv.promoteClient(client);
	std::cout << "PassCommand::execute(" << fds << ", " << pass << ")" << std::endl;
}
