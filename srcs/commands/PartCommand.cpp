#include "PartCommand.hpp"

PartCommand::PartCommand(void) :
_name("PART")
{
	std::cout << "┣⮕ PartCommand." << std::endl;
}

PartCommand::~PartCommand(void)
{}

bool
	PartCommand::cantExecute(Client *client)
{
	return (client->isRegistered());
}

void PartCommand::execute(int fds, Client *client)
{
	(void) fds;
	(void) client;
	Irc					&ircserv = Irc::getInstance();
	std::string				cmd =	client->getCmds().front();
	std::vector<std::string>		args = splitArguments(cmd);
	User					*user = ircserv.getUserByNick(client->getNick());
	std::map<std::string, std::string>	channels;


	user->quitAllChannel();
	std::cout << "[" << this->_name << "] : PartCommand executed !" << std::endl;
}
