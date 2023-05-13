#include "ft_irc.hpp"

Irc::~Irc()
{
	close(this->_sockfd);
}

Irc::Irc()
{
	*this = Irc(std::string("ircd"), std::string(""));
}

Irc::Irc(std::string port, std::string passwd): _pass(passwd)
{
	struct	addrinfo	hint;
	int			status;

	(void)port;
	std::memset(&hint, 0, sizeof(hint));
	hint.ai_family = AF_UNSPEC;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_flags = AI_PASSIVE;
	status = getaddrinfo(NULL, port.c_str(), &hint, &this->_net);
	if (status)
	{
		std::cerr << "getaddrinfo error: " << gai_strerror(status);
		throw SyscallError();
	}
 	
	if ((this->_sockfd = ft_setup_socket(this->_net)) == -1 || set_socket_option(this->_sockfd) == -1)
	{
		freeaddrinfo(this->_net);
		std::cerr << "Socket creation failed: ";
		throw SyscallError();
	}
	freeaddrinfo(this->_net);
}

// Getter
int	&Irc::getSocket() const {
	return (const_cast<int&>(this->_sockfd));
}

struct addrinfo	*Irc::getAi() const
{
	return (this->_net);
}

std::vector<User>	&Irc::getUsers() const
{
	return (const_cast<std::vector<User> &>(this->_users));
}

//Setter
void	Irc::addUser(int const &sfd)
{
	std::cout << "size of int ref" << sizeof(sfd) << std::endl;
}

int	Irc::computeFdMax(void) const
{
	return (this->_users.size() + 1);
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

Irc	&Irc::operator=(const Irc &rhs)
{
	int	status;

	status = getaddrinfo(NULL, NULL, rhs.getAi(), &this->_net);
	freeaddrinfo(this->_net);
	this->_sockfd = rhs.getSocket();
	this->_users = rhs.getUsers();
	return (*this);
}
