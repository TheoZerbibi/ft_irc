#include "ft_irc.hpp"

Irc::~Irc()
{
	if (this->_net)
		freeaddrinfo(_net);
	close(this->_sockfd);
}

Irc::Irc()
{
	*this = Irc("ircd", "");
}



Irc::Irc(char *port, char *passwd)
{
	struct	addrinfo	hint;
	int			status;

	std::memset(&hint, 0, sizeof(hint));
	hint.ai_family = AF_UNSPEC;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_flags = AI_PASSIVE;
	status = getaddrinfo(NULL, port, &hint, &this->_net);
	if (status)
	{
		std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
		throw SyscallError();
	}
	this->_sockfd = ft_setup_socket(this->net);
	if ((this->_sockfd = ft_setup_socket(this->_net)) == -1 || set_socket_option(this->_sockfd) == -1)
	{
		freeaddrinfo(this->_net);
		std::cerr << "Socket creation failed: " << std::endl;
		throw SyscallError();
	}
}

// Getter

int	&Irc::getSocket() const {
	return (this->_sockfd);
}

struct addrinfo	*Irc::getAi() const
{
	return (this->_net);
}

int	&Irc::getSocket() const
{
	return (this->_sockfd);
}

void	Irc::printAi() const
{
	std::cout << "Address family: ";
	if (_net->ai_family == AF_INET)
		std::cout << "Ipv4" << std::endl;
	else if (_net->ai_family == AF_INET6)
		std::cout << "Ipv6" << std::endl;
	else
		std::cout << "Unspecified" << std::endl;
	std::cout << "ip : " << inet_ntoa(((struct sockaddr_in *)_net->ai_addr)->sin_addr) << std::endl;
	std::cout << "Port : " << ntohs(((struct sockaddr_in *)_net->ai_addr)->sin_port) << std::endl;
}

Irc	Irc::operator=(Irc &rhs)
{
	int	status;

	status = getaddrinfo(NULL, NULL, rhs.getAi(), &this->_net);
	this->_sockfd = rhs.getSocket();
	this->_users = rhs.getUsers();
	return (*this);
}
