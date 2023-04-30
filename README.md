# Ft_Irc

##Requirement
- 

##Help

###Function

- socket() :

- htons / htonl : convert value given to a valid value for network using. Network value can only be big-endian style.
- ntohl / ntohs : Convert value Network-style representation to system representation (Big or Little endian)



- in_addr_t inet_addr(char \*addr) : Convert string representation of ipv4 address to binary representation for further usage.
- char \*inet_ntoa(struct \*in_addr in) : Convert struct adress to string representation for printing (buffer returned is static stored so further call will overwrite it).

- 

###Struct

```c++

	struct	addrinfo {
		int	ai_flags;		// AI_PASSIV, AI_CANNONAME, etc.
		int	ai_family;		// AF_INET (ip_v4), AF_INET6 (ip_v6, AF_UNSPEC (agnostic)
		int	ai_socktype;		//SOCK_STREAM (tcp), SOCK_DGRAM (udp)
		int	ai_protocol;		// use 0 for "any"
		size_t	ai_addrlen;		// size of ai_addr in bytes
		struct sockaddr *ai_addr;	// struct sockaddr_in or _in6 : hold port and ip address
		char	*ai_cannoname;		// full canonical hostname
		struct	addrinfo *ai_next;	// next node
	};

```

```c++

	struct	sockaddr_in {				// Wrap de sockaddr pour specifier le port et l'adresse de destination plus simplement;
		short int		sin_family;	// Famille de protocol ip comme pour addrinfo
		unsigned short int	sin_port;	// Port de destination
		struct	in_addr		sin_addr;	// addresse de destination
		unsigned char		sin_zero[8];	// Du padding plein de 0
	};

```

##External Ressource

