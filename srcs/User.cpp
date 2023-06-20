#include "User.hpp"

//	User::User(int fd, const std::string& nick, const std::string& username, const std::string& hostname): _fd(fd), _nick(nick), _username(username), _hostname(hostname)
//	{
//	}
//	
//	User::User(int &fd): _fd(fd)
//	{
//	}

Client::Client(int const &sfd)
{
	_sockFd = sfd;
	this->_isAuth = false;
	this->_registered = false;
	this->_nickname = "*";
}

Client::Client(Client *cpy)
{
	this->_sockFd = cpy->getSockfd();
	this->_nickname = cpy->getNick();
	this->_username = cpy->getUser();
	this->_hostname = cpy->getHost();
	this->_realname = cpy->getRealname();
	this->_isAuth = cpy->isAuth();
	this->_registered = cpy->isRegistered();
}

Client::Client():_sockFd(0)
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

void Client::readyToRegister()
{
	if (!this->_nickname.empty() && !this->_username.empty()
		&& !this->_hostname.empty() && !this->_realname.empty() && this->_isAuth) {
		Irc	&ircserv = Irc::getInstance();
		this->_registered = true;

		Reply rpl01 = Reply(this->_sockFd, RPL_WELCOME(ircserv.getName(), this->_nickname));
		Reply rpl02 = Reply(this->_sockFd, RPL_YOURHOST(ircserv.getName(), this->_nickname));
		Reply rpl03 = Reply(this->_sockFd, RPL_INFO(ircserv.getName(), this->_nickname));

		std::cout << "rpl01 - " << rpl01.getMessage() << "rpl02 - " << rpl02.getMessage() << "rpl03 - " << rpl03.getMessage() << std::endl;

		ircserv.addReply(rpl01);
		ircserv.addReply(rpl02);
		ircserv.addReply(rpl03);
	}
}

// 
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

std::deque<std::string> &Client::getCmds()
{
	return (this->_cmds);
}

void	Client::extractCmds()
{
	size_t		pos = 0;
	std::string	cmd;

	while ((pos = this->_buff.find(DELIM)) != std::string::npos)
	{
		std::cout << "found delimiter at pos : " << pos << std::endl;
		cmd = this->_buff.substr(0, pos);
		if (!cmd.empty())
			_cmds.push_back(cmd);
		_buff.erase(0, pos + sizeof(DELIM) - 1);
	}
}

User::User(Client *client): Client(*client)
{
	this->_registered = 1;
}

User::User()
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
