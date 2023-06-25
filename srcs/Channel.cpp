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
