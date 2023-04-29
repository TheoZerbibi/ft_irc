#include	<sys/types.h>
#include	<sys/socket.h>
#include	<arpa/inet.h>

#include	<cstring>
#include	<netdb.h>

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



void	setup_addrinfo(struct addrinfo *ai)
{
	std::memset(ai, 0, sizeof(*ai));
	ai->ai_family = AF_INET;
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
	struct	addrinfo *net;
	struct	addrinfo hint;

	setup_addrinfo(&hint);
	getaddrinfo(NULL, "ircd", &hint, &net);
	std::cout << "Port : " << ntohs(((struct sockaddr_in *)net->ai_addr)->sin_port) << std::endl;
	return (0);
}
