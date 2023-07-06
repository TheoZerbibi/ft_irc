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

void
	PartCommand::_leaveChannel(int fds, Client *client, std::vector<std::string> channels, std::string msg)
{
	Irc			&ircserv =	Irc::getInstance();
	User		*user = ircserv.getUserByNick(client->getNick());
	(void)fds;

	if (user == NULL) return ;
	for (unsigned int i = 0; i < channels.size(); i++)
	{
		if (!this->_chanIsValid(channels[i]))
		{
			ircserv.addReply(Reply(fds, ERR_BADCHANMASK(ircserv.getName(), client->getNick(), channels[i])));
			return ;
		}
		std::cout << "Leaving " << channels[i] << " with message " << msg << std::endl;
		if (ircserv.channelExists(channels[i]))
		{
			Channel *channel = ircserv.getChannel(channels[i]);
			if (!user->isOnChannel(channel)) {
				ircserv.addReply(Reply(fds, ERR_NOTONCHANNEL(ircserv.getName(), client->getNick(), channels[i])));
				return ;
			}
			user->quitChannel(channel, msg);
		}
		else
		{
			ircserv.addReply(Reply(fds, ERR_NOSUCHCHANNEL(ircserv.getName(), client->getNick(), channels[i])));
		}
	}
}

std::vector<std::string>
	PartCommand::_parseArg(std::vector<std::string> &args)
{
	std::vector<std::string>	channels;
	std::vector<std::string>	chans = splitStr(args.at(0), ',');

	for (unsigned int i = 0; i < chans.size(); i++)
	{
		if (chans[i].empty())
			continue ;
		channels.push_back(chans[i]);
	}
	return (channels);
}

void PartCommand::execute(int fds, Client *client)
{
	Irc							&ircserv =	Irc::getInstance();
	std::string					cmd =	client->getCmds().front();
	std::vector<std::string>	args = splitArguments(cmd);
	std::vector<std::string>	channels;
	std::string					msg;

	if (args.empty())
		return (ircserv.addReply(Reply(fds, ERR_NEEDMOREPARAMS(ircserv.getName(), client->getNick(), this->_name))));
	else {
		std::cout << "CMD : " << cmd << std::endl;
		std::cout << "ARG[0] : " << args[0] << std::endl;
		std::cout << "ARG[1] : " << args[1] << std::endl;
		if (args.size() >= 2)
			msg = args[1];
		try {
			channels = _parseArg(args);
			if (!channels.empty())
				this->_leaveChannel(fds, client, channels, msg);
		} catch (std::exception &e) {
			return ;
		}
	}
	std::cout << "[" << this->_name << "] : PartCommand executed !" << std::endl;
}

bool
	PartCommand::_chanIsValid(const std::string &name)
{
	if (name.empty())
		return (false);
	if (name[0] != '#' && name[0] != '&')
		return (false);
	return (true);
}
