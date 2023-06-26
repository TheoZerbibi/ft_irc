#include	"ft_irc.hpp"
#define		PREFIX "&#+!"

Channel::Channel(std::string name, std::string key, bool topicIsOpOnly, int maxUser) :
_operator(),
_users(),
_topic(),
_name(name),
_key(key),
_topicIsOpOnly(topicIsOpOnly),
_isInvis(false),
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

User	*Channel::getOper(std::string nick)
{
	std::map<std::string, User *>::iterator  beg = this->_operator.begin();
	std::map<std::string, User *>::iterator  end = this->_operator.end();

	while (beg != end)
	{
		if (beg->first == "@" + nick)
			return (beg->second);
	}
	return (NULL);
}

User	*Channel::getUser(std::string nick)
{
	std::map<std::string, User *>::iterator  beg = this->_users.begin();
	std::map<std::string, User *>::iterator  end = this->_users.end();

	while (beg != end)
	{
		if (beg->first == "@" + nick)
			return (beg->second);
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

bool		const &Channel::isInvis() const
{
	return (this->_isInvis);
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

void		Channel::setInvis(bool value)
{
	this->_isInvis = value;
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
		if (user)
		{
			removeUser(user);
			addOper(user);
		}
	}
	else
	{
		user = this->getOper(nick);
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
