#include "AwayCommand.hpp"

AwayCommand::AwayCommand(void) :
_name("AWAY")
{
	std::cout << "┣⮕ AwayCommand." << std::endl;
}

AwayCommand::~AwayCommand(void)
{}

bool
	AwayCommand::cantExecute(Client *client)
{
	return (client->isRegistered());
}

void AwayCommand::execute(int fds, Client *client)
{
	Irc							&ircserv = Irc::getInstance();
	std::string					cmd = client->getCmds().front();
	std::vector<std::string>	args = splitArguments(cmd);
	User						*user = ircserv.getUserByNick(client->getNickname());

	if (user->isAway()) {
		ircserv.addReply(Reply(fds, RPL_UNAWAY(ircserv.getName(), client->getNickname())));
		user->setAwayMessage("");
	}
	else if (args.size() > 0) {
		user->setAwayMessage(args[0]);
		ircserv.addReply(Reply(fds, RPL_NOWAWAY(ircserv.getName(), client->getNickname())));
	}
	else
		ircserv.addReply(Reply(fds, ERR_NEEDMOREPARAMS(ircserv.getName(), client->getNickname(), _name)));
	std::cout << "AwayCommand::execute(" << fds << ")" << std::endl;
}
