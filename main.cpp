#include	<sys/types.h>
#include	<sys/socket.h>
#include	<arpa/inet.h>
//#include	<netdb.h>

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

void	setup_sockaddr_in(struct sockaddr_in *sock, unsigned short int port, char *addr)
{
	sock->sin_family = AF_INET;
	sock->sin_port = htons(port);
	if (!(sock->sin_addr
//	if (!inet_pton(AF_INET, addr, &(sock->sin_addr)))
//		std::cout << "Addr wrong format: '" << addr << "'" << std::endl;
}

int	main(int ac, char **av)
{
	struct	addrinfo net;

	net.ai_family = AF_INET;
	return (0);
}
