#include "InviteCommand.hpp"

InviteCommand::InviteCommand(void) :
_name("INVITE")
{
	std::cout << "┣⮕ InviteCommand." << std::endl;
}

InviteCommand::~InviteCommand(void)
{}

bool
	InviteCommand::cantExecute(Client *client)
{
	return (client->isRegistered());
}

void InviteCommand::execute(int fds, Client *client)
{
	Irc					&ircserv =	Irc::getInstance();
	User					*user = dynamic_cast<User *>(client);
	std::string				cmd =	client->getCmds().front();
	std::vector<std::string>		args =	splitArguments(cmd);
	User					*target;
	Channel					*chan;

	if (args.size() < 2)
		return (dynamic_cast<User *>(client)->printInvited());
	target = ircserv.getUserByNick(args.at(0));
	if (!target)
		return (ircserv.addReply(Reply(fds, ERR_NOSUCHNICK(ircserv.getName(), args.at(0)))));
	if (!(chan = ircserv.getChannel(args.at(1))))
		return (ircserv.addReply(Reply(fds, ERR_NOSUCHCHANNEL(ircserv.getName(), user->getNickname(), args.at(1)))));
	if (!user->isOnChannel(chan))
		return (ircserv.addReply(Reply(fds, ERR_NOTONCHANNEL(ircserv.getName(), user->getNickname(), args.at(1)))));
	if (chan->isInvit() && !user->isChannelOper(chan))
		return (ircserv.addReply(Reply(fds, ERR_CHANOPRIVSNEEDED(ircserv.getName(), user->getNickname(), args.at(1)))));
	if (target->isOnChannel(chan))
		return (ircserv.addReply(Reply(fds, ERR_USERONCHANNEL(ircserv.getName(), user->getNickname(), args.at(1)))));
		
	target->inviteOnChannel(chan);
	ircserv.addReply(Reply(fds, RPL_INVITING(ircserv.getName(), user->getNickname(), args.at(0), args.at(1))));
	ircserv.addReply(Reply(target->getSockfd(), RPL_INVITE(user_id(ircserv.getName(), client->getNickname(), client->getUsername()), target->getNickname(), chan->getName())));
	std::cout << "[" << this->_name << "] : InviteCommand executed !" << std::endl;
}
