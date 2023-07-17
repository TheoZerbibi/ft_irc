#include "PrivMsgCommand.hpp"

PrivMsgCommand::PrivMsgCommand(void) :
_name("PRIVMSG")
{
	std::cout << "┣⮕ PrivMsgCommand." << std::endl;
}

PrivMsgCommand::~PrivMsgCommand(void)
{}


bool
	PrivMsgCommand::cantExecute(Client *client)
{
	return (client->isRegistered());
}

void PrivMsgCommand::execute(int fds, Client *client)
{
	(void) fds;
	(void) client;
	std::string			cmd = client->getCmds().front();
	std::vector<std::string>	args = splitArguments(cmd);
	Irc				&ircserv = Irc::getInstance();
	std::vector<std::string>	targets;

	if (args.size() < 2)
		return (ircserv.addReply(Reply(fds, ERR_NEEDMOREPARAMS(ircserv.getName(), client->getNickname(), this->_name))));
	targets = splitStr(args.at(0), ',');
	removeDuplicate(targets);

	this->sendAllMsg(dynamic_cast<User *>(client), targets, args.at(1));
	std::cout << "[" << this->_name << "] : PrivMsgCommand executed !" << std::endl;
}

void	PrivMsgCommand::sendMsg(User *user, std::string target, std::string msg)
{
	Irc				&ircserv = Irc::getInstance();
	Channel				*chan;
	User				*user_target;

	if (target.at(0) == '#')
	{
		if (!(chan = ircserv.getChannel(target))) {
			ircserv.addReply(Reply(user->getSockfd(), ERR_NOSUCHCHANNEL(ircserv.getName(), user->getNickname(), target)));
			return ;
		}
		std::cout << "isOn? " << user->isOnChannel(chan) << std::endl;
		if (user->isOnChannel(chan))
			chan->sendToChannel(user, RPL_PRIVMSG(user_id(ircserv.getName(), user->getNickname(), user->getUsername()), target, msg));
		else
			ircserv.addReply(Reply(user->getSockfd(), ERR_CANNOTSENDTOCHAN(ircserv.getName(), user->getNickname(), target)));
	}
	else
	{
		if (!(user_target = ircserv.getUserByNick(target)))
			return (ircserv.addReply(Reply(user->getSockfd(), ERR_NOSUCHNICK(ircserv.getName(), target))));
		ircserv.addReply(Reply(user_target->getSockfd(), RPL_PRIVMSG(user_id(ircserv.getName(), user->getNickname(), user->getUsername()), target, msg)));
		if (user_target->isAway())
			ircserv.addReply(Reply(user->getSockfd(), RPL_AWAY(ircserv.getName(), user->getNickname(), user_target->getNickname(), user_target->getAwayMessage())));
	}
}

void	PrivMsgCommand::sendAllMsg(User *user, std::vector<std::string> targets, std::string msg)
{
	std::vector<std::string>::iterator	beg = targets.begin();
	std::vector<std::string>::iterator	end = targets.end();

	while (beg != end)
	{
		this->sendMsg(user, *beg, msg);
		beg++;
	}
}


void	PrivMsgCommand::removeDuplicate(std::vector<std::string> &entry)
{
	std::vector<std::string>::iterator	beg = entry.begin();
	std::vector<std::string>::iterator	end = entry.end();
	std::vector<std::string>::iterator	it;

	while (beg != end)
	{
		it = beg + 1;
		while (it != end)
		{
			if (*beg == *it)
			{
				entry.erase(it++);
			}
			else
				it++;
		}
		beg++;
	}
}
