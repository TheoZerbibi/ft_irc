#include "ft_irc.hpp"

Irc::Irc()
{
}

Irc::~Irc()
{
	std::cout << "Freeing things" << std::endl;
	close(this->_sockfd);
	freeaddrinfo(this->_net);
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
		std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
		throw SyscallError();
	}
	if ((setup_socket()) == -1 || set_socket_option() == -1)
	{
		freeaddrinfo(this->_net);
		std::cerr << "Socket creation failed: ";
		throw SyscallError();
	}
}

// Getter
const int	&Irc::getSocket() const {
	return ((this->_sockfd));
}

const struct addrinfo	*Irc::getAi() const
{
	return (this->_net);
}

std::map<int, User>	&Irc::getUsers()
{
	return ((this->_users));
}

//Setter
void	Irc::addClient(int const &sfd)
{
	Client	client(sfd);

	_clients.insert(std::make_pair(sfd, client));
}

int	Irc::computeFdMax(void) const
{
	int	fdmax = this->getSocket();
	int	curr_fd;
	std::map<int, User>::const_iterator it = this->_users.begin();
	std::map<int, User>::const_iterator ite = this->_users.end();

	while (it != ite)
	{
		curr_fd = (*it).first;
		if (fdmax < curr_fd)
			fdmax = curr_fd;
		it++;
	}
	return (fdmax + 1);
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

//	Irc	&Irc::operator=(const Irc &rhs)
//	{
//		int	status;
//	
//		status = getaddrinfo(NULL, NULL, rhs.getAi(), &this->_net);
//		freeaddrinfo(this->_net);
//		this->_sockfd = rhs.getSocket();
//		this->_users = rhs.getUsers();
//		return (*this);
//	}
