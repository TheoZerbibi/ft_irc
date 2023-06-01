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
	this->_registered = false;
}

Client::Client(Client *cpy)
{
	this->_sockFd = cpy->getSockfd();
	this->_nickname = cpy->getNick();
	this->_username = cpy->getUser();
	this->_hostname = cpy->getHost();
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

const std::string	&Client::getBuff() const
{
	return (this->_buff);
}

const int	&Client::getSockfd() const
{
	return (this->_sockFd);
}


bool const	&Client::isRegistered() const
{
	return(this->_registered);
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
