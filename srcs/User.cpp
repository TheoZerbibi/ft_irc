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

const int	&Client::getSockfd() const
{
	return (this->_sockFd);
}


// 

bool Client::recvData()
{
	int	nbyte;
	char	buff[2];

	// Get data utile there is no more data to retrieve
	
	bzero(buff, sizeof(buff));
	if ((nbyte = recv(this->_sockFd, buff, sizeof(buff), 0)) <= 0)
	{
		std::cout << "Error with recv" << std::endl;
		if (nbyte < 0)
		{
			//Error from recv, maybe print something
		}
		close(this->_sockFd);
		return (0);
	}
	this->_buff += buff;
	std::cout << "fd =" << this->_sockFd << ": " << this->_buff << std::endl;
	write(this->_sockFd, this->_buff.c_str(), this->_buff.size());
	return (SUCCESS);
}

User::User(Client &client)
{
	this->_sockFd = client.getSockfd();
	this->_nickname = client.getNick();
	this->_username = client.getUser();
	this->_hostname = client.getHost();
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
