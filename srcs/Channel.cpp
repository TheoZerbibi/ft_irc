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
	std::string 		memberList;
	std::string		nick;
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

bool		Channel::isEmpty() const
{
	std::cout << "CHECKING CHANNEL EMPTYNESS" << std::endl;
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
	std::pair<std::string, User *> user_entry = std::make_pair<std::string, User*>(user->getNick(), user);
	this->_users.insert(user_entry);
	this->printUserList();
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

void		Channel::setInvit(bool value)
{
	this->_isInvit = value;
}

void		Channel::removeOper(User *user, std::string reason)
{
	this->removeOper(user->getNick(), reason);
}

void		Channel::removeOper(std::string nick, std::string reason)
{
	Irc												&ircserv = Irc::getInstance();
	std::map<std::string, User *>::iterator	found =	_operator.find('@' + nick);
	std::map<std::string, User *>::iterator	end =	_operator.end();

	if (found != end)
	{
		ircserv.addReply(Reply(found->second->getSockfd(), RPL_PART(user_id(ircserv.getName(), nick, found->second->getUser()), this->_name, reason)));
		this->sendToChannel(found->second, RPL_PART(user_id(ircserv.getName(), nick, found->second->getUser()), this->_name, reason));

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
	this->removeUser(user->getNick(), reason);
}

void		Channel::removeUser(std::string nick, std::string reason)
{
	Irc										&ircserv = Irc::getInstance();
	std::map<std::string, User *>::iterator	found = _users.find(nick);
	std::map<std::string, User *>::iterator	end = _users.end();


	std::cout << "||--> deciding user status : " << nick << std::endl;
	if (found != end)
	{
		std::cout << "||-->removing user : " << nick << std::endl;

		ircserv.addReply(Reply(found->second->getSockfd(), RPL_PART(user_id(ircserv.getName(), nick, found->second->getUser()), this->_name, reason)));
		this->sendToChannel(found->second, RPL_PART(user_id(ircserv.getName(), nick, found->second->getUser()), this->_name, reason));
		_users.erase(found);
	} else
		this->removeOper(nick, reason);
}

void		Channel::setOper(std::string nick, bool value)
{
	User	*user;
	Irc		&ircserv = Irc::getInstance();

	if (value)
	{
		user = this->getUser(nick);
		if (user)
		{
			this->_users.erase(nick);
			addOper(user);
			this->sendToEveryone(user, RPL_MODEWITHARG(user_id(ircserv.getName(), user->getNick(), user->getUser()), this->_name, "+o", user->getNick()));
		}
	}
	else
	{
		user = this->getOper(nick);
		if (user)
		{
			this->_operator.erase(nick);
			addUser(user);
			this->sendToEveryone(user, RPL_MODEWITHARG(user_id(ircserv.getName(), user->getNick(), user->getUser()), this->_name, "-o", user->getNick()));
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

	this->printUserList();
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
