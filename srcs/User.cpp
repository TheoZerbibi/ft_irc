#include "User.hpp"


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
_nickname(cpy->getNickname()),
_hostname(cpy->getHost()),
_username(cpy->getUsername()),
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

User::User():
	_chans(),
	_invited(),
	_awayMessage(),
	_isOper(false),
	_isInvis(false)
{
}

User::~User()
{
	std::cout << "||-->" << this->_nickname << " is leaving the server" << std::endl;
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

bool			User::isOnChannel(Channel *chan)
{
	if (getChannel(chan) == _chans.end())
		return (false);
	return (true);
}

const std::string	&Client::getNickname() const
{
	return (this->_nickname);
}

const std::string	&Client::getUsername() const
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

bool	User::_isChannelUser(Channel *chan) const
{
	if (chan->getUser(this->_nickname))
		return (true);
	return (false);
}

bool	User::isInvited(Channel *chan)
{
	std::vector<Channel *>::iterator beg = _invited.begin();
	std::vector<Channel *>::iterator end = _invited.end();

	while (beg != end)
	{
		if (*beg == chan)
			return (true);
	}
	return (false);
}

void
	User::removeInvite(Channel *chan)
{
	std::vector<Channel *>::iterator	it = _invited.begin();
	std::vector<Channel *>::iterator	end = _invited.end();

	while (it != end)
	{
		if (*it == chan)
		{
			std::cout << "JOIN : user is invited" << std::endl;
			this->_invited.erase(it);
			return ;
		}
		it++;
	}
}

void	User::printInvited()
{
	Irc	&ircserv = Irc::getInstance();

	std::vector<Channel *>::iterator it = _invited.begin();
	std::vector<Channel *>::iterator end = _invited.end();

	while (it != end)
	{
		ircserv.addReply(Reply(_sockFd, RPL_INVITELIST(ircserv.getName(), _nickname, (*it)->getName())));
		it++;
	}
	ircserv.addReply(Reply(_sockFd, RPL_ENDOFINVITELIST(ircserv.getName(), _nickname)));
}

void	User::inviteOnChannel(Channel *chan)
{
	std::vector<Channel *>::iterator  beg = _invited.begin();
	std::vector<Channel *>::iterator  end = _invited.end();

	while (beg != end)
	{
		if (*beg == chan)
			return ;
	}
	_invited.push_back(chan);
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
	char	buff[2048];

	// Get data utile there is no more data to retrieve

	bzero(buff, sizeof(buff));
	if ((nbyte = recv(this->_sockFd, buff, sizeof(buff), 0)) <= 0)
	{
		if (nbyte < 0)
		{
			std::cerr << "Error with recv" << std::endl;
			//Error from recv, maybe print something
		}
		close(this->_sockFd);
		return (0);
	}
	this->_buff += buff;
	this->extractCmds();
	std::cout << "=== Cmd list ==" << std::endl;
	this->printCmds();
//	std::cout << "Buffer = \'" << this->_buff << "\'" << std::endl;
	return (SUCCESS);
}

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

void	User::removeChannel(Channel *chan)
{
	this->_chans.erase(getChannel(chan));
}

void	User::quitChannel(Channel *chan, std::string const &msg)
{
	std::cout << "||-->" << this->_nickname << " Quiting channel : " << chan->getName() << "with msg : " << msg << std::endl;
	chan->removeUser(this->_nickname, msg);
	this->_chans.erase(getChannel(chan));
}

void	User::quitAllChannel(std::string const &msg)
{
	std::vector<Channel *>::iterator	beg = _chans.begin();
	std::vector<Channel *>::iterator	end = _chans.end();

	while (beg != end)
	{
		this->quitChannel(*beg, msg);
		beg++;
	}
}


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
	_invited(),
	_awayMessage(),
	_isOper(false),
	_isInvis(false)
{
	this->_registered = true;
}

//Printer


void	User::printInfo()
{
	std::cout << "-------UserInfo-------" << std::endl;
	std::cout << "nick: " << _nickname << std::endl;
	printChannels();
	std::cout << "----------------------" << std::endl;
}


void	User::printChannels()
{
	std::vector<Channel *>::iterator	beg = _chans.begin();
	std::vector<Channel *>::iterator	end = _chans.end();

	std::cout << "Channels :: ";
	while (beg != end)
	{
		std::cout << (*beg)->getName() << " ";
		beg++;
	}
	std::cout << std::endl;
}

const std::string
	&User::getAwayMessage(void) const
{
	return (this->_awayMessage);
}

void
	User::setAwayMessage(std::string const &awayMessage)
{
	this->_awayMessage = awayMessage;
}

bool
	User::isAway(void) const
{
	return (!this->_awayMessage.empty());
}
