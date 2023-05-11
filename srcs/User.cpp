#include "User.hpp"

User::User(int fd, const std::string& nick, const std::string& username, const std::string& hostname): m_fd(fd), m_nick(nick), m_username(username), m_hostname(hostname)
{
}

User::~User()
{
}

int	User::getFd() const
{
	return (this->m_fd);
}

const std::string& User::getNick() const
{
	return (this->m_nick);
}

const std::string& User::getUsername() const
{
	return (this->m_username);
}

const std::string& User::getHostname() const
{
	return (this->m_hostname);
}
