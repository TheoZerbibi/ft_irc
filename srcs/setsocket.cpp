#include "ft_irc.hpp"

void	printAi(struct addrinfo *net)
{
	std::cout << "Address family: ";
	if (net->ai_family == AF_INET)
		std::cout << "Ipv4" << std::endl;
	else if (net->ai_family == AF_INET6)
		std::cout << "Ipv6" << std::endl;
	else
		std::cout << "Unspecified" << std::endl;
	std::cout << "ip : " << inet_ntoa(((struct sockaddr_in *)net->ai_addr)->sin_addr) << std::endl;
	std::cout << "Port : " << ntohs(((struct sockaddr_in *)net->ai_addr)->sin_port) << std::endl;
}

int	Irc::setup_socket()
{
	if ((this->_sockfd = socket(_net->ai_family, _net->ai_socktype, _net->ai_protocol)) == -1)
		return (-1);
	if (bind(this->_sockfd, this->_net->ai_addr, this->_net->ai_addrlen) == -1)
	{
		return (-1);
	}
	if (listen(this->_sockfd, 3) == -1)
		return (-1);
	return (_sockfd);
}

int	Irc::set_socket_option()
{
	int	truevalue = 1;
	struct	linger	ling = (struct linger){.l_onoff = 1, .l_linger = 2};

	if (setsockopt(this->_sockfd, SOL_SOCKET, SO_REUSEADDR, &truevalue, sizeof(int)) == -1) //Permit instant rebind on port when socket is closed (for fast restart)
		return (-1);
	if (setsockopt(this->_sockfd, SOL_SOCKET, SO_KEEPALIVE, &truevalue, sizeof(int)) == -1) //Enable monitoring of connection health
		return (-1);
	if (setsockopt(this->_sockfd, SOL_SOCKET, SO_LINGER, &ling, sizeof(linger)) == -1) //Setup waiting time for sending data when socket is closed
		return (-1);
	if (setsockopt(this->_sockfd, IPPROTO_TCP, TCP_NODELAY, &truevalue, sizeof(int)) == -1)//Disable Nagle algorithm which optimize network by buffering small packet
		return (-1);
	return (0);
}
