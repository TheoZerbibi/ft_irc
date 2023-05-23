#pragma		once
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<netinet/tcp.h>
#include	<arpa/inet.h>

#include	<unistd.h> //need to be deleted and replaced by recv and send

#include	<csignal>

#include	<cstring>
#include	<netdb.h>

#include	<errno.h>

#include	<iostream>
#include	<algorithm>

#include	<vector>
#include	<map>
#include	<sys/select.h>

#include	"User.hpp"


// SELECT FD ENUM
enum
{
	MASTER,
	READ,
	SEND,
	EXCEPT
};

class	Irc{
	private:
		int			_sockfd;
		struct	addrinfo	*_net;
		std::string		_pass;
//		std::vector<Channel>	chans;

		//Setup socket interface
		int	setup_socket();
		int	set_socket_option();

		// Client, User 
		std::map<int, Client>	_clients; // Unregistered User, got promoted to User after
		std::map<int, User>	_users; // Maybe put nickname as first pair to facilitate user interaction
						
		std::vector<Channel>	_channels;

		// Const & destr
		Irc();
		Irc(std::string port, std::string passwd);

		// Private Copy and assignement to enforce singularity
		Irc(const Irc &);
		Irc &operator=(const Irc &);
	public:
		//Const & destr
		


		static Irc &getInstance() // Init and retrieve server instance
		{
			static Irc	instance("ircd", "");
			return instance;
		}
		~Irc();

		//Utils
		void			printAi() const;

		//Setter
		void			addUser(int const &sfd);
		void			addClient(int const &sfd);

		//Getter
		const	int			&getSocket() const;
		const	struct addrinfo		*getAi() const;
		const	std::map<int, User>	&getUsers() const;
		int				computeFdMax() const;
};

class	Channel
{
	public:
		Channel(std::string	&name);
		~Channel();

		void	addUser(User &_user);
		getUser(std::string _nick);
	private:
		Channel();
		std::string			_name;
		std::vector<int, std::string>	_operator;
		std::vector<int, std::string>	_users;
};

class	SyscallError: public std::exception
{
	const char *what() const throw(){
		return (strerror(errno));
	}
};

struct	addrinfo *setup_addrinfo(char *port);
