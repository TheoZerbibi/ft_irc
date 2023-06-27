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
	std::cout << "Delete " << this->_name << " channel." << std::endl;
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

std::string const
	Channel::getUsersNick() const
{
	std::string 				nick_str = "";
	std::map<std::string, User *>::const_iterator	beg =	_operator.begin();
	std::map<std::string, User *>::const_iterator	end =	_operator.end();

	while (beg != end)
	{
		if (!nick_str.empty())
			nick_str += " ";
		nick_str += beg->first;
		++beg;
	}
	beg = _users.begin();
	end = _users.end();
	while (beg != end)
	{
		if (!nick_str.empty())
			nick_str += " ";
		nick_str += beg->first;
		++beg;

	}
	return (nick_str);
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
	std::string 							memberList;
	std::string								nick;
	std::map<std::string, User *>::const_iterator it;

	for (it = this->_users.begin(); it != this->_users.end(); it++)
	{
		nick.clear();
		nick = it->second->getNick();
		if (it->second->isInvis())
			continue;

		memberList += nick;
		memberList += " ";
	}

	for (it = this->_operator.begin(); it != this->_operator.end(); it++)
	{
		nick.clear();
		nick = it->second->getNick();
		if (it->second->isInvis())
			continue;

		memberList += "@" + nick;
		memberList += " ";
	}
	return (memberList);
}


//Setter
void		Channel::addUser(User *user)
{
	std::pair<std::string, User *> user_entry = std::make_pair<std::string, User*>(user->getNick(), user);
	this->_users.insert(user_entry);
}

void		Channel::addOper(User *user)
{
	std::pair<std::string, User *> user_entry = std::make_pair<std::string, User*>("@" + user->getNick(), user);
	this->_operator.insert(user_entry);
	user->addChannel(this);
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
	this->_maxUser = value;
}

void		Channel::setInvit(bool value)
{
	this->_isInvit = value;
}

void		Channel::removeOper(User *user, std::string reason)
{
	std::string nick = user->getNick();
	this->removeOper(nick, reason);
}

void		Channel::removeOper(std::string nick, std::string reason)
{
	std::map<std::string, User *>::iterator	found = _operator.find('@' + nick);
	std::map<std::string, User *>::iterator	end = _operator.end();
	Irc										&ircserv = Irc::getInstance();
	if (found != end)
	{
		_operator.erase(found);
		if (_operator.size() == 0 && _users.size() == 0) {
			ircserv.removeChannel(this);
			return ;
		}
		if (_operator.size() == 0) {
			std::map<std::string, User *>::iterator	found = _users.begin();
			std::cout << "Op User = " << found->second->getNick() << std::endl;
			this->addOper(found->second);
		}
		std::cout << found->second->getSockfd() << std::endl;
		ircserv.addReply(Reply(found->second->getSockfd(), RPL_PART(ircserv.getName(), user_id(ircserv.getName(), nick, found->second->getUser()), this->_name, reason)));
		this->sendToChannel(found->second, RPL_PART(ircserv.getName(), user_id(ircserv.getName(), nick, found->second->getUser()), this->_name, reason));
	}
}

void		Channel::removeUser(User *user, std::string reason)
{
	std::string	nick = user->getNick();
	this->removeUser(nick, reason);
}

void		Channel::removeUser(std::string nick, std::string reason)
{
	std::map<std::string, User *>::iterator	found = _users.find(nick);
	std::map<std::string, User *>::iterator	end = _users.end();
	Irc										&ircserv = Irc::getInstance();

	if (found != end)
	{
		_users.erase(found);
		ircserv.addReply(Reply(found->second->getSockfd(), RPL_PART(ircserv.getName(), user_id(ircserv.getName(), nick, found->second->getUser()), this->_name, reason)));
		this->sendToChannel(found->second, RPL_PART(ircserv.getName(), user_id(ircserv.getName(), nick, found->second->getUser()), this->_name, reason));
	} else
		this->removeOper(nick);
}

void		Channel::setOper(std::string nick, bool value)
{
	User *user;

	if (value)
	{
		user = this->getUser(nick);
		std::cout << "found User = " << user << std::endl;
		if (user)
		{
			removeUser(user);
			addOper(user);
		}
	}
	else
	{
		user = this->getOper(nick);
		std::cout << "found Oper = " << user << std::endl;
		if (user)
		{
			removeOper(user);
			addUser(user);
		}
	}
}

void		Channel::setTopicMode(bool value)
{
	this->_topicIsOpOnly = value;
}


void
	Channel::sendToChannel(User *user, std::string msg) {
	std::map<std::string, User *>	userList;
	Irc								&ircserv = Irc::getInstance();

	userList.insert(this->_users.begin(), this->_users.end());
	userList.insert(this->_operator.begin(), this->_operator.end());

	std::map<std::string, User *>::iterator beg = userList.begin();
	std::map<std::string, User *>::iterator end = userList.end();

	while (beg != end)
	{
		if (beg->second != user)
			 ircserv.addReply(Reply(beg->second->getSockfd(), msg));
			// ircserv.addReply(Reply(beg->second->getSocket(), RPL_PRIVMSG(ircserv.getName(), user_id(ircserv.getName(), user->getNick(), user->getUser()), this->_name, msg)));
		beg++;
	}
}
