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


std::vector<std::string> splitArguments(const std::string &cmd)
{
	std::vector<std::string> tokens;
	std::string		tmp;
	std::size_t start = 0, end = 0;

	while (start != std::string::npos)
	{
		if (start != 0 && cmd.at(start + 1) == ':')
		{
			tokens.push_back(cmd.substr(start + 2));
			return (tokens);
		}
		start = (!start ? start : start + 1);
		end = cmd.find(' ', start);     // if not find end == npos, it's ok
		tokens.push_back(cmd.substr(start, end - start)); //it will simply substr until end of cmd
		start = end;
	}
	return (tokens);
}

// Probleme !
// tokenize apres une occurence de :
// Ignore totalement le dernier token lorsqu'il n'est pas suivie par au moins un espace
// string::substr prend une len en tant que second argument et non pas une position

//	std::vector<std::string> splitArguments(const std::string &cmd)
//	{
//		std::vector<std::string> tokens;
//		std::size_t start = 0, end = 0;
//	
//		while ((end = cmd.find(' ', start)) != std::string::npos) {
//			if (end != start) {
//				std::string tmp = cmd.substr(start, end - start);
//				if (tmp[0] == ':') {
//					tmp = cmd.substr(start, end);
//					tmp = tmp.substr(1);
//				}
//				std::cout << tmp << std::endl;
//				tokens.push_back(tmp);
//			}
//			start = end + 1;
//		}
//		return tokens;
//	}
