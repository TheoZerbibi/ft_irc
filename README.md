# Ft_Irc
-


### Client management

After creating the main listening socket we will have to start managing client.
Things to do are :

- Accepting new connexion from client: registering them as client
- listening to incoming data from client and buffering them in personnal buffer
- Reading buffer from client to extract command *Optionnal*
- Queuing command
- executing command

## Structure

### Global

```c++

class ft_irc {
	private:
		std::vector<User>	member;
		std::vector<Channel>	chan;
};

```

```c++

class Channel {
	private:
		std::string		name;
		std::vector<int>	operators;
		std::vector<int>	users;
};

```

### User

``` c++

class User {
	private:
		int		sockfd;
		char		cmdbuff[512];
		std::string	name;
		std::string	nick;
		std::string	host;
	public:
		// Construct & Destructor
		// Setter & Getter
};

```


### Channel
- Channel names are string (beginning with a '&', a '#', ...)

## Command and RPL

### Registration

> Immediately upon establishing a connection the client must attempt registration, without waiting for any banner message from the server.

> Until registration is complete, only a limited subset of commands SHOULD be accepted by the server. This is because it makes sense to require a registered (fully connected) client connection before allowing commands such as JOIN, PRIVMSG and others.


| Command | Description | Reply | Error |
|---------|-------------|-------|-------|
| NICK nickname|        | Reply | Error |
| Command | Description | Reply | Error |
| Command | Description | Reply | Error |
| Command | Description | Reply | Error |


## Help

### Function

- socket() :

- htons / htonl : convert value given to a valid value for network using. Network value can only be big-endian style.
- ntohl / ntohs : Convert value Network-style representation to system representation (Big or Little endian)


- in_addr_t inet_addr(char \*addr) : Convert string representation of ipv4 address to binary representation for further usage.
- char \*inet_ntoa(struct \*in_addr in) : Convert struct adress to string representation for printing (buffer returned is static stored so further call will overwrite it).

### Struct

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

## Requirement

- [ ] Server / client socket connection
 - [ ] Accept multiple client and register them under a user id

- [ ] Command management
 - [ ] Get command on user-sockfd (Should use a buffer for eachuser)
 - [ ] Parse command

>
>
- [ ] Signal handling
 - [ ] CTRL+C
 - [ ] CTRL+D
 - [ ] CTRL+Z


- [ ] Define client structure including: socket-fd, Id, command buffer
- [ ] Parse client input 
 - [ ] Make a get_next_command to get command
- [ ] Define rooms with userid

- [ ] Server command
 - [ ]  PING
 - [ ]  OPER
 - [ ]  KILL

- [ ] Implement RPL system
 - [ ] Implement ERR

### Extra

- [ ] Do a script to automate connexion on server with nc

## External Ressource

- [Reference Repo](https://github.com/marineks/Ft_irc)
- [ModernIrc](https://modern.ircdocs.horse/)
- [Network programming guide](https://beej.us/guide/bgnet/pdf/bgnet_a4_c_1.pdf)
- [Irssi Doc](https://irssi.org/New-users/)
