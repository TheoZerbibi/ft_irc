#include "UserCommand.hpp"



UserCommand::UserCommand(void) :
_name("USER")
{
	std::cout << "┗⮕ UserCommand." << std::endl;
}

UserCommand::~UserCommand(void)
{}

void UserCommand::execute(int fds, Client *client)
{
	std::string	cmd = client->getCmds().front();
	std::vector<std::string> arguments = splitArguments(cmd);
	Irc	&ircserv = Irc::getInstance();

	if (client->isRegistered())
		ircserv.addReply(Reply(fds, ERR_ALREADYREGISTERED(ircserv.getName(), client->getNick())));
	else if (arguments.size() != 5)
		ircserv.addReply(Reply(fds, ERR_NEEDMOREPARAMS(ircserv.getName(), client->getNick(), this->_name)));
	else {
		client->setUser(arguments[1]);
		client->setHost(arguments[3]);
		client->setRealname(arguments[4]);
		std::cout << "UserCommand::execute(" << fds << ", " << arguments[1] << ", " << arguments[2] << ", " << arguments[3] << ", " << arguments[4] << ")" << std::endl;
		client->readyToRegister();
	}
}

std::vector<std::string> splitArguments(const std::string &cmd) {
	std::vector<std::string> tokens;
	std::size_t start = 0, end = 0;

	while ((end = cmd.find(' ', start)) != std::string::npos) {
		if (end != start) {
			std::string tmp = cmd.substr(start, end - start);
			if (tmp[0] == ':') {
				tmp = cmd.substr(start, end);
				tmp = tmp.substr(1);
			}
			std::cout << tmp << std::endl;
			tokens.push_back(tmp);
		}
		start = end + 1;
	}
	return tokens;
}
