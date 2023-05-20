#include "User.hpp"

//	User::User(int fd, const std::string& nick, const std::string& username, const std::string& hostname): _fd(fd), _nick(nick), _username(username), _hostname(hostname)
//	{
//	}
//	
//	User::User(int &fd): _fd(fd)
//	{
//	}

Client::Client()
{
}

Client::Client(int &sfd): _sockFd(sfd)
{
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
