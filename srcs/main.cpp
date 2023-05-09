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


//	void	ft_read_socket(int sfd)
//	{
//		struct addrinfo net;
//		struct sockaddr	sa;
//	
//		net.ai_addr = &sa;
//		std::vector<int> fd;
//		int	newfd = accept(sfd, net.ai_addr, &net.ai_addrlen);
//		if (newfd == -1)
//		{
//			std::cout << strerror(errno) << std::endl;
//			freeaddrinfo(&net);
//			return;
//		}
//		fd.push_back(newfd);
//		std::cout << newfd << std::endl;
//		while (1)
//		{
//			read(newfd, buff, 512);
//			std::cout << buff;
//			bzero(buff, 512);
//		}
//	}

int	main(int ac, char **av)
{
	(void)av;
	(void)ac;
	Irc	ircserv;
//	int			sfd = 5;
//	struct	addrinfo	*net;
//	
//	if (!(net = setup_addrinfo(av[1])))
//		return (1);
//	if ((sfd = ft_setup_socket(net)) == -1 || set_socket_option(sfd) == -1)
//	{
//		std::cout << strerror(errno) << std::endl;
//		freeaddrinfo(net);
//		return (1);
//	}
	ircserv.printAi();
//	ft_read_socket(sfd);

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
	return (0);
}
