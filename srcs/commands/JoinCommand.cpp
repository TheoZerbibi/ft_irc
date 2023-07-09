#include "JoinCommand.hpp"

JoinCommand::JoinCommand(void) :
_name("JOIN")
{
	std::cout << "┣⮕ JoinCommand." << std::endl;
}

JoinCommand::~JoinCommand(void)
{}

void
	JoinCommand::_joinChannel(int fds, Client *client, std::map<std::string , std::string> channels)
{
	Irc			&ircserv =	Irc::getInstance();
	User		*user = ircserv.getUserByNick(client->getNick());

	if (user == NULL)
		return ;
	for (std::map<std::string , std::string>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		std::cout << "Joining " << it->first << " with key " << it->second << std::endl;
		if (!this->_chanIsValid(it->first))
		{
			ircserv.addReply(Reply(fds, ERR_BADCHANMASK(ircserv.getName(), client->getNick(), it->first)));
			return ;
		}
		if (ircserv.channelExists(it->first))
		{
			Channel *channel = ircserv.getChannel(it->first);
			std::cout << "Channel" << channel->getName() << " exists" << std::endl;
			if (channel->isInvit() && !user->isInvited(channel))
				return (ircserv.addReply(Reply(fds, ERR_INVITEONLYCHAN(ircserv.getName(), client->getNick(), channel->getName()))));
			if (!channel->getKey().empty() && channel->getKey() != it->second)
				return (ircserv.addReply(Reply(fds, ERR_BADCHANNELKEY(ircserv.getName(), client->getNick(), channel->getName()))));
			std::cout << "JOIN :Removing " << user->getNick() << " From invite list of " << channel->getName() << std::endl;
			user->removeInvite(channel);
			std::cout << "JOIN : end of desinviting" << std::endl;
			user->addChannel(channel);
			channel->addUser(user);
			rplJoin(fds, user, channel);
		}
		else
		{
			std::cout << "JOIN : Creating channel " << it->first << std::endl;
			Channel *channel = ircserv.addChannel(it->first);
			if (channel == NULL) {
				ircserv.addReply(Reply(fds, ERR_BADCHANMASK(ircserv.getName(), client->getNick(), it->first)));
				return ;
			}
			if (it->second != "x")
				channel->setKey(it->second);
			user->addChannel(channel);
			channel->addOper(user);
			rplJoin(fds, user, channel);
		}
	}
}

std::map<std::string, std::string>	JoinCommand::_parseArg(std::vector<std::string> &args)
{
	std::map<std::string, std::string>	channels;
	std::pair<std::string, std::string>	chans_pair;
	std::vector<std::string>			chans = splitStr(args.at(0), ',');
	std::vector<std::string>			key;
	args.erase(args.begin());
	if (!args.empty())
		key = splitStr(args.at(0), ',');

	std::vector<std::string>::iterator	chans_beg = chans.begin();
	std::vector<std::string>::iterator	chans_end = chans.end();
	while (chans_beg != chans_end)
	{
		if (key.empty())
			chans_pair = make_pair(*chans_beg, "x");
		else
		{	
			chans_pair = make_pair(*chans_beg, *key.begin());
			key.erase(key.begin());
		}
		channels.insert(chans_pair);
		chans_beg++;
	}
	return (channels);
}

bool
	JoinCommand::cantExecute(Client *client)
{
	return (client->isRegistered());
}

void JoinCommand::execute(int fds, Client *client)
{
	Irc									&ircserv =	Irc::getInstance();
	std::string							cmd =	client->getCmds().front();
	std::vector<std::string>			args = splitArguments(cmd);
	std::map<std::string, std::string>	channels;
	User								*user = ircserv.getUserByNick(client->getNick());

	if (!user)
		return ;
	if (args.empty())
	{
		return (ircserv.addReply(Reply(fds, ERR_NEEDMOREPARAMS(ircserv.getName(), client->getNick(), this->_name))));
	}
	else if (args.at(0) == "0")
		user->quitAllChannel();
	else
	{
		try {
			channels = _parseArg(args);
			if (!channels.empty())
				this->_joinChannel(fds, client, channels);
		} catch (std::exception &e) {
			return ;
		}
	}
	std::cout << "JoinCommand::execute(" << fds << ", " << client->getNick() << ")" << std::endl;
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

void
	JoinCommand::rplJoin(int fds, User *user, Channel *chan)
{
	Irc &ircserv = Irc::getInstance();

	chan->sendToEveryone(user, RPL_JOIN(user_id(ircserv.getName(), user->getNick(), user->getUser()), chan->getName()));
	if (!chan->getTopic().empty())
		ircserv.addReply(Reply(fds, RPL_TOPIC(ircserv.getName(), user->getNick(), user->getUser(), chan->getTopic())));
	ircserv.addReply(Reply(fds, RPL_NAMREPLY(ircserv.getName(), user->getNick(), chan->getType(), chan->getName(), chan->getMemberList())));
	ircserv.addReply(Reply(fds, RPL_ENDOFNAMES(ircserv.getName(), user->getNick())));
}

