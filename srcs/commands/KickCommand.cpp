#include "KickCommand.hpp"

KickCommand::KickCommand(void) :
_name("KICK")
{
	std::cout << "┣⮕ KickCommand." << std::endl;
}

KickCommand::~KickCommand(void)
{}


bool
	KickCommand::cantExecute(Client *client)
{
	return (client->isRegistered());
}

void
	KickCommand::_kickUser(Client *executor, std::map<Channel *, User *> kicks, std::string reason)
{
	Irc			&ircserv =	Irc::getInstance();
	User		*executorUser = ircserv.getUserByNick(executor->getNick());
	if (executorUser == NULL)
		return ;
	for (std::map<Channel *, User *>::iterator it = kicks.begin(); it != kicks.end(); it++)
	{
		Channel	*channel = it->first;
		User	*target = it->second;
		std::cout << "Kick " << target->getUser() << " from channel " << channel->getName() << " with reason : " << reason << std::endl;
		if (!executorUser->isChannelOper(channel))
		{
			ircserv.addReply(Reply(executor->getSockfd(), ERR_CHANOPRIVSNEEDED(ircserv.getName(), executor->getNick(), channel->getName())));
			return ;
		}
		if (!target->isOnChannel(channel))
		{
			ircserv.addReply(Reply(executor->getSockfd(), ERR_NOTONCHANNEL(ircserv.getName(), executor->getNick(), channel->getName())));
			return ;
		}
		if (target->isChannelOper(channel)) return ;
		// channel->kickUser(executorUser, target, reason);
	}
}

std::map<Channel *, User *>
	KickCommand::_parseArg(Client *executor, std::vector<std::string> &args)
{
	Irc								&ircserv =	Irc::getInstance();
	std::map<Channel *, User *>		kicks;
	std::pair<Channel *, User *>	kicksPair;
	std::vector<std::string>		kickChannel = splitStr(args.at(0), ',');
	std::vector<std::string>		kickUser;

	args.erase(args.begin());
	if (args.empty())
	{
		ircserv.addReply(Reply(executor->getSockfd(), ERR_NEEDMOREPARAMS(ircserv.getName(), executor->getNick(), this->_name)));
		throw (std::exception());
	}
	kickUser = splitStr(args.at(0), ',');

	if (kickChannel.empty() || kickUser.empty())
	{
		ircserv.addReply(Reply(executor->getSockfd(), ERR_NEEDMOREPARAMS(ircserv.getName(), executor->getNick(), this->_name)));
		throw (std::exception());
	}
	if (kickChannel.size() > kickUser.size() && kickUser.size() != 1) {
		ircserv.addReply(Reply(executor->getSockfd(), ERR_NEEDMOREPARAMS(ircserv.getName(), executor->getNick(), this->_name)));
		throw (std::exception());
	}

	std::vector<std::string>::iterator	kickChannel_beg = kickChannel.begin();
	std::vector<std::string>::iterator	kickChannel_end = kickChannel.end();

	while (kickChannel_beg != kickChannel_end)
	{
		Channel *channel = ircserv.getChannel(*kickChannel_beg);
		if (channel) {
			User *user = ircserv.getUserByNick(*kickUser.begin());
			if (!user)
			{
				ircserv.addReply(Reply(executor->getSockfd(), ERR_NOSUCHNICK(ircserv.getName(), *kickUser.begin())));
				throw (std::exception());
			}
			kicksPair = std::make_pair(channel, user);
			if (kickUser.size() > 1)
				kickUser.erase(kickUser.begin());
		} else {
			ircserv.addReply(Reply(executor->getSockfd(), ERR_NOSUCHCHANNEL(ircserv.getName(), executor->getNick(), *kickChannel_beg)));
			throw (std::exception());
		}
		kicks.insert(kicksPair);
		kickChannel_beg++;
	}
	return (kicks);
}

/**
** -> KICK #test1,#test2 to,tba :Adios Bahamas
** -> KICK #test to :You should not stay !
** -> KICK #test to,tba :Your friend is in another channel!
** -> KICK #test to
**/
void
	KickCommand::execute(int fds, Client *client)
{
	Irc							&ircserv =	Irc::getInstance();
	std::string					cmd =	client->getCmds().front();
	std::vector<std::string>	args = splitArguments(cmd);
	std::map<Channel *, User *>	kicks;
	User						*user = ircserv.getUserByNick(client->getNick());
	std::string					reason;

	if (!user)
		return ;
	if (args.empty())
		return (ircserv.addReply(Reply(fds, ERR_NEEDMOREPARAMS(ircserv.getName(), client->getNick(), this->_name))));
	else {
		std::cout << "ARG[0] : " << args[0] << std::endl;
		std::cout << "ARG[1] : " << args[1] << std::endl;
		std::cout << "ARG[2] : " << args[2] << std::endl;
		if (args.size() >= 2)
			reason = args[2];
		else
			reason = "Kicked by the channel's operator";
		try {
			kicks = _parseArg(client, args);
			if (!kicks.empty())
				this->_kickUser(client, kicks, reason);
		} catch (std::exception &e) {
				return ;
		}
	}
	std::cout << "KickCommand::execute(" << fds << ", " << client->getNick() << ")" << std::endl;
}
