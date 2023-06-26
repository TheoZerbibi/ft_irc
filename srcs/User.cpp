#include "User.hpp"

//	User::User(int fd, const std::string& nick, const std::string& username, const std::string& hostname): _fd(fd), _nick(nick), _username(username), _hostname(hostname)
//	{
//	}
//	
//	User::User(int &fd): _fd(fd)
//	{
//	}

Client::Client(int const &sfd):
_buff(),
_cmds(),
_sockFd(sfd),
_isAuth(false),
_registered(false),
_nickname("*"),
_hostname(),
_username(),
_realname()
{}

Client::Client(Client *cpy):
_buff(),
_cmds(),
_sockFd(cpy->getSockfd()),
_isAuth(cpy->isAuth()),
_registered(cpy->isRegistered()),
_nickname(cpy->getNick()),
_hostname(cpy->getHost()),
_username(cpy->getUser()),
_realname(cpy->getRealname())
{
}

Client::Client():
_buff(),
_cmds(),
_sockFd(0),
_isAuth(false),
_registered(false),
_nickname("*"),
_hostname(),
_username(),
_realname()
{
}

Client::~Client()
{
}

// Setter
void	Client::setNick(std::string nick)
{
	this->_nickname = nick;
}

void	Client::setUser(std::string user)
{
	this->_username = user;
}

void	Client::setHost(std::string host)
{
	this->_hostname = host;
}

void	Client::setRealname(std::string realname)
{
	this->_realname = realname;
}


// Getter

std::vector<Channel *>::iterator	User::getChannel(Channel *chan)
{
	std::vector<Channel *>::iterator	beg = _chans.begin();
	std::vector<Channel *>::iterator	end = _chans.end();

	while (beg != end)
	{
		if (*beg == chan)
			return (beg);
		beg++;
	}
	return (end);
}

bool					User::isOnChannel(Channel *chan)
{
	if (getChannel(chan) == _chans.end())
		return (false);
	return (true);
}

const std::string	&Client::getNick() const
{
	return (this->_nickname);
}

const std::string	&Client::getUser() const
{
	return (this->_username);
}

const std::string	&Client::getHost() const
{
	return (this->_hostname);
}

const std::string	&Client::getRealname() const
{
	return (this->_realname);
}

const std::string	&Client::getBuff() const
{
	return (this->_buff);
}

const int	&Client::getSockfd() const
{
	return (this->_sockFd);
}

bool User::isOper() const
{
	return (_isOper);
}

bool User::isInvis() const
{
	return (_isInvis);
}

bool	User::isChannelOper(Channel *chan) const
{
	if (chan->getOper(this->_nickname))
		return (true);
	return (false);
}

bool	User::isChannelUser(Channel *chan) const
{
	if (chan->getUser(this->_nickname))
		return (true);
	return (false);
}

void	Client::setAuth(bool auth)
{
	this->_isAuth = auth;
}

bool const	&Client::isAuth() const
{
	return (this->_isAuth);
}

bool const	&Client::isRegistered() const
{
	return(this->_registered);
}

bool Client::recvData()
{
	int	nbyte;
	char	buff[512];

	// Get data utile there is no more data to retrieve

	bzero(buff, sizeof(buff));
	if ((nbyte = recv(this->_sockFd, buff, sizeof(buff), 0)) <= 0)
	{
		if (nbyte < 0)
		{
			std::cout << "Error with recv" << std::endl;
			//Error from recv, maybe print something
		}
		close(this->_sockFd);
		return (0);
	}
	this->_buff += buff;
	this->extractCmds();
	std::cout << "=== Cmd list ==" << std::endl;
	this->printCmds();
	std::cout << "Buffer = \'" << this->_buff << "\'" << std::endl;
	return (SUCCESS);
}

//	void	User::joinChannel(std::string channame, std::string key)
//	{
//	
//	}


void	User::addChannel(Channel *chan)
{
	this->_chans.push_back(chan);
}

void	User::joinChannel(std::string chanName, std::string key)
{
	Irc	&ircserv = Irc::getInstance();
	Channel	*chan = ircserv.getChannel(chanName);
	(void)key;

	if (!chan)
	{
		chan = ircserv.addChannel(chanName);
		chan->addOper(this);
	}
	chan->addUser(this);
}

void	User::quitChannel(Channel *chan)
{
	chan->removeOper(this);
	chan->removeUser(this);
}


//	void	User::joinChannelsByNames(std::vector<std::string>::iterator beg_chans, std::vector<std::string>::iterator end_chans)
//	{
//		while (beg_chans != end_chans)
//		{
//			this->joinChannel(*beg_chans, "");
//			beg_chans++;
//		}
//	}

std::deque<std::string> &Client::getCmds()
{
	return (this->_cmds);
}

// Utils
void	Client::printCmds()
{
	std::deque<std::string>::iterator beg = _cmds.begin();
	std::deque<std::string>::iterator end = _cmds.end();

	while (beg != end)
	{
		std::cout << *beg << std::endl;
		beg++;
	}
}

void	Client::extractCmds()
{
	size_t		pos = 0;
	std::string	cmd;

	while ((pos = this->_buff.find(DELIM)) != std::string::npos)
	{
		cmd = this->_buff.substr(0, pos);
		if (!cmd.empty())
			_cmds.push_back(cmd);
		_buff.erase(0, pos + sizeof(DELIM) - 1);
	}
}


User::User(Client *client):
Client(*client),
_chans(),
_isOper(false),
_isInvis(false)
{
	this->_registered = true;
}

User::User():
_chans(),
_isOper(false),
_isInvis(false)
{
}

User::~User()
{
}

//	const int	&User::getFd() const
//	{
//		return (this->_fd);
//	}
//	
//	const std::string& User::getNick() const
//	{
//		return (this->_nick);
//	}
//	
//	const std::string& User::getUsername() const
//	{
//		return (this->_username);
//	}
//	
//	const std::string& User::getHostname() const
//	{
//		return (this->_hostname);
//	}
