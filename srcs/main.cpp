#include "ft_irc.hpp"

bool	ft_check_input(int ac, char **av)
{
	std::string		port;
	std::string::iterator	beg;
	std::string::iterator	end;

	if (ac != 3)
	{
		std::cout << "Wrong usage: format must be './ircserv <port> <password>'" << std::endl;
		return (1);
	}
	port = av[1];
	beg = port.begin(), end = port.end();
	while (beg != end)
	{
		if (!std::isdigit(*beg))
		{
			std::cout << "Wrong port format: '" << port << "' should only contain digit" << std::endl;
			return (1);
		}
		beg++;
	}
	return (0);
}

void	print_ai(struct addrinfo *ai)
{
	std::cout << "Address family: ";
	if (ai->ai_family == AF_INET)
		std::cout << "Ipv4" << std::endl;
	else if (ai->ai_family == AF_INET6)
		std::cout << "Ipv6" << std::endl;
	else
		std::cout << "Unspecified" << std::endl;
	std::cout << "ip : " << inet_ntoa(((struct sockaddr_in *)ai->ai_addr)->sin_addr) << std::endl;
	std::cout << "Port : " << ntohs(((struct sockaddr_in *)ai->ai_addr)->sin_port) << std::endl;
}


void	ft_read_socket(int sfd)
{
	struct addrinfo net;
	struct sockaddr	sa;
	char	buff[512] = "";

	net.ai_addr = &sa;
		std::vector<int> fd;

		int	newfd = accept(sfd, net.ai_addr, &net.ai_addrlen);
		if (newfd == -1)
		{
			std::cout << strerror(errno) << std::endl;
			freeaddrinfo(&net);
			return;
		}
		fd.push_back(newfd);
		std::cout << newfd << std::endl;
	while (1)
	{
		read(newfd, buff, 512);
		std::cout << buff;
		bzero(buff, 512);
	}
}

int	main(int ac, char **av)
{
	(void)av;
	(void)ac;
	int			sfd = 5;
	struct	addrinfo	*net;

	if (!(net = setup_addrinfo(av[1])))
		return (1);

	if ((sfd = ft_setup_socket(net)) == -1 || set_socket_option(sfd) == -1)
	{
		std::cout << strerror(errno) << std::endl;
		freeaddrinfo(net);
		return (1);
	}
	print_ai(net);

	ft_read_socket(sfd);

	//	recv(newfd, buff, 10, 0);
	//	while (1)
	//	{
	//		char	*line;
	//		while (get_next_line(newfd, & line))
	//		{
	//			std::cout << "line = " << line << std::endl;
	//			if (!strcmp(line, "exit"))
	//			{
	//				free(line);
	//				freeaddrinfo(net);
	//				return (0);
	//			}
	//			free(line);
	//		}
	//	}

	//	if (connect(sfd, net->ai_addr, net->ai_addrlen) == -1)
	//	{
	//		std::cout << "wut" << std::endl;
	//		std::cout << strerror(errno) << std::endl;
	//		freeaddrinfo(net);
	//		return (1);
	//	}

	freeaddrinfo(net);
	return (0);
}
