#include "JoinCommand.hpp"

JoinCommand::JoinCommand(void) :
_name("JOIN")
{
	std::cout << "┣⮕ JoinCommand." << std::endl;
}

JoinCommand::~JoinCommand(void)
{}

void	JoinCommand::joinChannels(int fds, User *user, std::vector<std::string> &channels, std::vector<std::string> *chans_key)
{
	
}

void	JoinCommand::replyToSuccefulJoin(int fds, User *user, Channel *chan)
{
}

//	void JoinCommand::execute(int fds, Client *client)
//	{
//		std::string	cmd =	client->getCmds().front();
//		size_t		pos =	cmd.find(" ");
//		std::string	arg =	cmd.substr(cmd.find(" ") + 1);
//		Irc		&ircserv =	Irc::getInstance();
//	
//		if (arg == cmd)
//			arg.clear();
//		std::cout << "POS = " << pos << std::endl;
//		std::cout << "[JoinCommand] CMD = " << cmd << std::endl << "ARG = " << arg << std::endl;
//		if (!arg.empty() && arg[0] == '0')
//			std::cout << "Leave all channel for " << client->getNick() << std::endl;
//		else if (arg.empty() || !this->_parseArguments(arg))
//		{
//			ircserv.addReply(Reply(fds, ERR_NEEDMOREPARAMS(ircserv.getName(), client->getNick(), this->_name)));
//			return ;
//		}
//	}

void JoinCommand::execute(int fds, Client *client)
{
	std::string			cmd =	client->getCmds().front();
	std::vector<std::string>	args =	splitArguments(cmd);
	std::vector<std::string>	channels;
	std::vector<std::string>	chans_key;
	Irc				&ircserv =	Irc::getInstance();

	if (args.empty())
	{
		ircserv.addReply(Reply(fds, ERR_NEEDMOREPARAMS(ircserv.getName(), client->getNick(), this->_name)));
		return ;
	}
	channels = splitStr(args.at(0), ',');
	args.erase(args.begin());
	if (args.empty())
		return (joinChannels(fds, dynamic_cast<User *>(client), channels, NULL));
	chans_key = splitStr(args.at(0), ',');
	joinChannels(fds, dynamic_cast<User *>(client), channels, &chans_key);
}

bool JoinCommand::_parseArguments(std::string str)
{
	if (str.empty())
		return (false);
	if (str[0] == ' ')
		str.erase(0, 1);
	if (str.find(" ") != str.npos)
		str = str.substr(0, str.find(" "));
	for (size_t i = 0; i < str.size(); i++)
	{
		if (isalpha(str[i]))
			return (true);
	}
	return (false);
}

/*
** #foo -> Public chan - Anyone can join.
** &foo -> Private chan - Only invited users can join.
*/

bool JoinCommand::_isJoinable(const std::string &channel)
{
	if (channel.empty())
		return (false);
	if (channel[0] != '#' && channel[0] != '&')
		return (false);
	return (true);
}
