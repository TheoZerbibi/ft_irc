#include "JoinCommand.hpp"

JoinCommand::JoinCommand(void) :
_name("JOIN")
{
	std::cout << "┣⮕ JoinCommand." << std::endl;
}

JoinCommand::~JoinCommand(void)
{}

std::map<std::string , std::string>
	JoinCommand::_parseCommand(int fds, Client *client, std::string str) {
	std::map<std::string , std::string>	channels;
	std::vector<std::string>			splitArgs;
	std::vector<std::string>			channelName;
	std::vector<std::string>			keys;
	Irc									&ircserv =	Irc::getInstance();

	splitArgs = this->splitStr(str, ' ');
	std::vector<std::string>::iterator it = splitArgs.begin();

	channelName = this->splitStr(*it, ',');
	for (size_t i = 0; i < channelName.size(); i++)
	{
		if (!this->_chanIsValid(channelName[i])) {
			ircserv.addReply(Reply(fds, ERR_BADCHANMASK(ircserv.getName(), client->getNick(), channelName[i])));
			throw (std::exception());
		}
	}
	if (it != splitArgs.end())
	{
		it++;
		keys = this->splitStr(*it, ',');
		for (size_t i = 0; i < keys.size(); i++)
		{
			if (!this->_passwordIsValid(keys[i])) {
				ircserv.addReply(Reply(fds, ERR_BADCHANNELKEY(ircserv.getName(), client->getNick(), channelName[i])));
				throw (std::exception());
			}
		}
	} else {
		for (size_t i = 0; i < channelName.size(); i++)
			keys.push_back("x");
	}
	for (size_t i = 0; i < channelName.size(); i++)
	{
		channels[channelName[i]] = keys[i];
	}

	return (channels);
}

void
	JoinCommand::execute(int fds, Client *client)
{
	std::string	cmd =	client->getCmds().front();
	std::string	arg =	cmd.substr(cmd.find(" ") + 1);
	Irc			&ircserv =	Irc::getInstance();
	
	if (arg == cmd)
		arg.clear();
	if (!arg.empty() && arg[0] == '0')
		std::cout << "Leave all channel for " << client->getNick() << std::endl;
	else if (arg.empty())
	{
		ircserv.addReply(Reply(fds, ERR_NEEDMOREPARAMS(ircserv.getName(), client->getNick(), this->_name)));
		return ;
	} else {
		try {
			std::map<std::string , std::string> channels = this->_parseCommand(fds, client, arg);
		} catch (std::exception &e) {
			return ;
		}
	}
}

/*
** #foo -> Public chan - Anyone can join.
** &foo -> Private chan - Only invited users can join.
*/

bool
	JoinCommand::_chanIsValid(const std::string &name)
{
	if (name.empty())
		return (false);
	if (name[0] != '#' && name[0] != '&')
		return (false);
	return (true);
}

bool JoinCommand::_passwordIsValid(std::string &password) {
	if (password.empty())
		return (false);
	if (password[0] == ':')
		password.erase(password.begin());
	if (password == "x")
		return (true);
	if (password.size() > 50 || password.size() < 3)
		return (false);
	return (true);
}
