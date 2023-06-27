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


char		const &Channel::getType()
{
	return (this->_type);
}

User	*Channel::getOper(std::string nick)
{
	std::map<std::string, User *>::iterator  beg = this->_operator.begin();
	std::map<std::string, User *>::iterator  end = this->_operator.end();

	while (beg != end)
	{
		if (beg->first == "@" + nick)
			return (beg->second);
		++beg;
	}
	return (NULL);
}

std::string	Channel::getUsersNick()
{
	std::string 				nick_str = "";
	std::map<std::string, User *>::iterator	beg =	_operator.begin();
	std::map<std::string, User *>::iterator	end =	_operator.end();

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
	std::map<std::string, User *>::iterator  beg = this->_users.begin();
	std::map<std::string, User *>::iterator  end = this->_users.end();

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

Client const
	*Channel::clientExist(Client *client) const
{
	std::map<std::string, User *>::const_iterator  beg = this->_users.begin();
	std::map<std::string, User *>::const_iterator  end = this->_users.end();

	while (beg != end)
	{
		if (beg->second == client)
			return (beg->second);
		beg++;
	}

	beg = this->_operator.begin();
	end = this->_operator.end();
	while (beg != end)
	{
		if (beg->second == client)
			return (beg->second);
		beg++;
	}
	return (NULL);
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

void		Channel::removeOper(User *user)
{
	std::string nick = user->getNick();
	this->removeOper(nick);
}

void		Channel::removeOper(std::string nick)
{
	std::map<std::string, User *>::iterator  found = _operator.find('@' + nick);
	std::map<std::string, User *>::iterator  end = _operator.end();
	if (found != end)
	{
		_operator.erase(found);
	}
}

void		Channel::removeUser(User *user)
{
	std::string	nick = user->getNick();
	this->removeUser(nick);
}

void		Channel::removeUser(std::string nick)
{
	std::map<std::string, User *>::iterator  found = _users.find(nick);
	std::map<std::string, User *>::iterator  end = _users.end();

	if (found != end)
	{
		_users.erase(found);
	}
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
