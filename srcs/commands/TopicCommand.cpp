#include "TopicCommand.hpp"

TopicCommand::TopicCommand(void) :
_name("TOPIC")
{
	std::cout << "┣⮕ TopicCommand." << std::endl;
}

TopicCommand::~TopicCommand(void)
{}

bool
	TopicCommand::cantExecute(Client *client)
{
	return (client->isRegistered());
}

//void TopicCommand::executeTopic(int fds, User *client, Channel *chan)
void TopicCommand::execute(int fds, Client *client)
{
	std::string			cmd = client->getCmds().front();
	std::vector<std::string>	args = splitArguments(cmd);
	Irc				&ircserv = Irc::getInstance();
	User				*user = dynamic_cast<User *>(client);
	Channel				*chan;
	std::string			topic;

	if (args.empty())
		return (ircserv.addReply(Reply(fds, ERR_NEEDMOREPARAMS(ircserv.getName(), client->getNickname(), this->_name))));
	if (!(chan = ircserv.getChannel(args.at(0))))
		return (ircserv.addReply(Reply(fds, ERR_NOSUCHCHANNEL(ircserv.getName(), client->getNickname(), args.at(0)))));
	if (!user->isOnChannel(chan))
		return (ircserv.addReply(Reply(fds, ERR_NOTONCHANNEL(ircserv.getName(), client->getNickname(), args.at(0)))));
	if (chan->topicIsRestricted() && !chan->getOper(user->getNickname()))
		return (ircserv.addReply(Reply(fds, ERR_CHANOPRIVSNEEDED(ircserv.getName(), client->getNickname(), args.at(0)))));
	args.erase(args.begin());
	if (args.empty())
	{
		if (chan->getTopic() == "")
			return (ircserv.addReply(Reply(fds, RPL_NOTOPIC(ircserv.getName(), client->getNickname(), chan->getName()))));
		return (ircserv.addReply(Reply(fds, RPL_TOPIC(ircserv.getName(), client->getNickname(), chan->getName(), chan->getTopic()))));
	}
	chan->setTopic(args.front());
	chan->sendToEveryone(user, RPL_TOPICSET(user_id(ircserv.getName(), user->getNickname(), user->getUsername()), chan->getName(), args.front()));
}
