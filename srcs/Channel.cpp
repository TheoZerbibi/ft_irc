#include	"ft_irc.hpp"
#define		PREFIX "&#+!"

Channel::Channel(std::string name, std::string key, bool topicIsOpOnly, int maxUser) :
_operator(),
_users(),
_topic(),
_name(name),
_key(key),
_type('='),
_topicIsOpOnly(topicIsOpOnly),
_isInvit(false),
_maxUser(maxUser)
{
}

Channel::~Channel()
{
}

//Getter
std::string const &Channel::getName() const
{
	return (this->_name);
}

char		const &Channel::getType() const
{
	return (this->_type);
}

User	*Channel::getOper(std::string nick)
{
	std::map<std::string, User *>::iterator	beg = this->_operator.begin();
	std::map<std::string, User *>::iterator	end = this->_operator.end();

	while (beg != end)
	{
		if (beg->first == "@" + nick)
			return (beg->second);
		++beg;
	}
	return (NULL);
}

User	*Channel::getUser(std::string nick)
{
	std::map<std::string, User *>::const_iterator	beg = this->_users.begin();
	std::map<std::string, User *>::const_iterator	end = this->_users.end();
	while (beg != end)
	{
		if (beg->first == nick)
			return (beg->second);
		beg++;
	}

	beg = this->_operator.begin();
	end = this->_operator.end();
	while (beg != end)
	{
		if (beg->first == nick)
			return (beg->second);
		beg++;
	}
	return (NULL);
}

std::string	const &Channel::getKey() const
{
	return (this->_key);
}

int		const &Channel::getLimit() const
{
	return (this->_maxUser);
}

bool		const &Channel::topicIsRestricted() const
{
	return (this->_topicIsOpOnly);
}

std::string	const &Channel::getTopic() const
{
	return (this->_topic);
}

bool		const &Channel::isInvit() const
{
	return (this->_isInvit);
}

std::string const
	Channel::getMemberList() const
{
	std::string 		memberList;
	std::string		nick;
	std::map<std::string, User *>::const_iterator it;

	for (it = this->_users.begin(); it != this->_users.end(); it++)
	{
		nick.clear();
		nick = it->second->getNickname();
		if (it->second->isInvis())
			continue;

		memberList += nick;
		memberList += " ";
	}

	for (it = this->_operator.begin(); it != this->_operator.end(); it++)
	{
		nick.clear();
		nick = it->second->getNickname();
		if (it->second->isInvis())
			continue;

		memberList += "@" + nick;
		memberList += " ";
	}
	return (memberList);
}

bool		Channel::isEmpty() const
{
	if (_operator.empty() && _users.empty())
		return (true);
	return (false);
}

bool		Channel::noOper() const
{
	if (_operator.empty())
		return (true);
	return (false);
}


//Setter
void		Channel::addUser(User *user)
{
	std::pair<std::string, User *> user_entry = std::make_pair<std::string, User*>(user->getNickname(), user);
	this->_users.insert(user_entry);
	this->printUserList();
}

void		Channel::addOper(User *user)
{
	std::pair<std::string, User *> user_entry = std::make_pair<std::string, User*>("@" + user->getNickname(), user);
	this->_operator.insert(user_entry);
}

void		Channel::setTopic(std::string topic)
{
	this->_topic = topic;
}

void		Channel::setKey(std::string key)
{
	this->_key = key;
}

void		Channel::setMaxUser(int value)
{
	if (value >= 0)
		this->_maxUser = value;
}

bool		Channel::isFull() const
{
	int	totalUser = this->_operator.size() + this->_users.size();

	if (_maxUser > 0)
	{
		if (totalUser >= _maxUser)
			return (true);
	}
	return (false);
}

void		Channel::setInvit(bool value)
{
	this->_isInvit = value;
}

void		Channel::removeOper(User *user, std::string reason)
{
	this->removeOper(user->getNickname(), reason);
}

void		Channel::removeOper(std::string nick, std::string reason)
{
	Irc												&ircserv = Irc::getInstance();
	std::map<std::string, User *>::iterator	found =	_operator.find('@' + nick);
	std::map<std::string, User *>::iterator	end =	_operator.end();

	if (found != end)
	{
		if (reason != "kicked")
		{
			ircserv.addReply(Reply(found->second->getSockfd(), RPL_PART(user_id(ircserv.getName(), nick, found->second->getUsername()), this->_name, reason)));
			this->sendToChannel(found->second, RPL_PART(user_id(ircserv.getName(), nick, found->second->getUsername()), this->_name, reason));
		}

		_operator.erase(found);

		if (this->_operator.empty() && this->_users.empty()) {
			std::cout << "REMOVING CHANNEL : " << this->_name << std::endl;
			ircserv.removeChannel(this);
		}
		else if (this->_operator.empty())
			this->fillOperPos();
	}
}

void		Channel::fillOperPos()
{
	std::cout << "PROMOTING :" << this->_users.begin()->first << " at Oper position" << std::endl;
	this->setOper(this->_users.begin()->first, ADDING);
}

void		Channel::removeUser(User *user, std::string reason)
{
	this->removeUser(user->getNickname(), reason);
}

void		Channel::removeUser(std::string nick, std::string reason)
{
	Irc										&ircserv = Irc::getInstance();
	std::map<std::string, User *>::iterator	found = _users.find(nick);
	std::map<std::string, User *>::iterator	end = _users.end();


	if (found != end)
	{
		if (reason != "kicked")
		{
			ircserv.addReply(Reply(found->second->getSockfd(), RPL_PART(user_id(ircserv.getName(), nick, found->second->getUsername()), this->_name, reason)));
			this->sendToChannel(found->second, RPL_PART(user_id(ircserv.getName(), nick, found->second->getUsername()), this->_name, reason));
		}
		this->_users.erase(found);
		if (this->_operator.empty() && this->_users.empty()) {
			std::cout << "REMOVING CHANNEL : " << this->_name << std::endl;
			ircserv.removeChannel(this);
		}
	} else
		this->removeOper(nick, reason);
}

	void
Channel::kickUser(User *executor, User *target, std::string reason)
{
	Irc			&ircserv =	Irc::getInstance();
	if (!target->isOnChannel(this) || !executor->isOnChannel(this))
	{
		ircserv.addReply(Reply(executor->getSockfd(), ERR_NOTONCHANNEL(ircserv.getName(), executor->getNickname(), this->_name)));
		return ;
	}
	if (!this->getOper(executor->getNickname()))
	{
		ircserv.addReply(Reply(executor->getSockfd(), ERR_CHANOPRIVSNEEDED(ircserv.getName(), executor->getNickname(), this->_name)));
		return ;
	}
	this->sendToEveryone(target, RPL_KICK(user_id(ircserv.getName(), executor->getNickname(), executor->getUsername()), this->_name, target->getNickname(), reason));
	target->removeChannel(this);
	this->removeUser(target, "kicked");
}

void		Channel::setOper(std::string nick, bool value)
{
	User	*user;

	if (value)
	{
		user = this->getUser(nick);
		if (user)
		{
			this->_users.erase(nick);
			addOper(user);
		}
	}
	else
	{
		user = this->getOper(nick);
		if (user)
		{
			this->_operator.erase("@" + nick);
			addUser(user);
		}
	}
}

void		Channel::setTopicMode(bool value)
{
	this->_topicIsOpOnly = value;
}


void	Channel::sendToEveryone(User *user, std::string msg)
{
	Irc				&ircserv = Irc::getInstance();

	sendToChannel(user, msg);
	ircserv.addReply(Reply(user->getSockfd(), msg));
}


void
Channel::sendToChannel(User *user, std::string msg) {
	std::map<std::string, User *>	userList;
	Irc				&ircserv = Irc::getInstance();

	userList.insert(this->_users.begin(), this->_users.end());
	userList.insert(this->_operator.begin(), this->_operator.end());

	std::map<std::string, User *>::iterator beg = userList.begin();
	std::map<std::string, User *>::iterator end = userList.end();

	while (beg != end)
	{
		if (beg->second != user)
			ircserv.addReply(Reply(beg->second->getSockfd(), msg));
		beg++;
	}
}

	void
Channel::printUserList(void)
{
	std::map<std::string, User *>	userList;
	std::cout << "List all user" << std::endl;
	userList.insert(this->_users.begin(), this->_users.end());
	userList.insert(this->_operator.begin(), this->_operator.end());

	std::map<std::string, User *>::iterator beg = userList.begin();
	std::map<std::string, User *>::iterator end = userList.end();
	while (beg != end)
	{
		beg->second->printInfo();
		beg++;
	}
}
