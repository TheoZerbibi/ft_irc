#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<arpa/inet.h>

#include	<cstring>
#include	<netdb.h>

#include	<errno.h>

#include	<iostream>
#include	<algorithm>

//	typedef struct	addrinfo {
//		int	ai_flags;
//		int	ai_family;
//		int	ai_socktype;
//		int	ai_protocol;
//		size_t	ai_addrlen;
//		struct sockaddr *ai_addr;
//		char	*ai_cannoname;
//		struct	addrinfo *ai_next;
//	}		t_ainfo;

//	struct	sockaddr_in {    // Wrap de sockaddr pour specifier le port et l'adresse de destination plus simplement;
//		short int		sin_family;	// Famille de protocol ip comme pour addrinfo
//		unsigned short int	sin_port;	// Port de destination
//		struct	in_addr		sin_addr;	// addresse de destination
//		unsigned char		sin_zero[8];	// Du padding plein de 0
//	};

//	void	setup_sockaddr_in(struct sockaddr_in *sock, unsigned short int port, char *addr)
//	{
//		sock->sin_family = AF_UNSPEC;
//		sock->sin_port = htons(port);
//		if (!(sock->sin_addr = inet_addr(INADDR_ANY)))
//			std::cout << "Could not translate addr" << std::endl;
//	//	if (!inet_pton(AF_INET, addr, &(sock->sin_addr)))
//	//		std::cout << "Addr wrong format: '" << addr << "'" << std::endl;
//	}

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

void	setup_addrinfo(struct addrinfo *ai)
{
	std::memset(ai, 0, sizeof(*ai));
	ai->ai_family = AF_UNSPEC;
	ai->ai_socktype = SOCK_STREAM;
	ai->ai_flags = AI_PASSIVE;
}

bool	ft_check_input(int ac, char **av)
{
	std::string	port;
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

int	main(int ac, char **av)
{
	(void)av;
	(void)ac;
	struct	addrinfo	*net;
	struct	sockaddr_in	sa;
	struct	addrinfo	hint;
	int			sfd;
	int			status;

	hint.ai_addr = (struct  sockaddr *)&sa;
	setup_addrinfo(&hint);
	if ((status = getaddrinfo("127.0.0.1", "10024", &hint, &net)))
	{
		std::cout << "getaddinfo error: " << gai_strerror(status) << std::endl;
		return (1);
	}
	if ((sfd = socket(net->ai_family, net->ai_socktype, net->ai_protocol)) == -1)
	{
		std::cout << strerror(errno) << std::endl;
		freeaddrinfo(net);
		return (1);
	}
	print_ai(net);
	std::cout << "FD : " << sfd << std::endl;
	if (bind(sfd, net->ai_addr, net->ai_addrlen) == -1)
	{
		std::cout << strerror(errno) << std::endl;
		freeaddrinfo(net);
		return (1);
	}
	if (listen(sfd, 1) == -1)
	{
		std::cout << strerror(errno) << std::endl;
		freeaddrinfo(net);
		return (1);
	}
	
	int	newfd = accept(sfd, net->ai_addr, &net->ai_addrlen);
	if (newfd == -1)
	{
		std::cout << strerror(errno) << std::endl;
		freeaddrinfo(net);
		return (1);
	}
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
