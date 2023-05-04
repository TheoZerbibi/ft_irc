#include "User.hpp"

User::User(int fd, const std::string& nick, const std::string& username, const std::string& hostname): m_fd(fd), m_nick(nick), m_username(username), m_hostname(hostname)
{
}

~User::User()
{
}

int	User::getFd() const
{
	return (this->m_fd);
}

const std::string& getNick() const
{
	return (this->m_nick);
}

const std::string& getUsername() const
{
	return (this->m_username);
}

const std::string& getHostname() const;
{
	return (this->m_hostname);
}
