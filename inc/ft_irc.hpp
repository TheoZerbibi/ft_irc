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
		std::map<int, User>	_users;
		// Const & destr
		Irc();
		Irc(std::string port, std::string passwd);

		// Private Copy and assignement to enforce singularity
		Irc(const Irc &);
		Irc &operator=(const Irc &);
	public:
		//Const & destr
		static Irc &getInstance()
		{
			static Irc	instance("ircd", "");
			return instance;
		}
		~Irc();

		//Utils
		void			printAi() const;

		std::vector<int>	_clients;

		//Setter
		void			addUser(int const &sfd);

		//Getter
		const	int			&getSocket() const;
		const	struct addrinfo		*getAi() const;
		const	std::map<int, User>	&getUsers() const;
		int			computeFdMax() const;
};

class	SyscallError: public std::exception
{
	const char *what() const throw(){
		return (strerror(errno));
	}
};

int	ft_setup_socket(struct addrinfo *net);
struct	addrinfo *setup_addrinfo(char *port);
int	set_socket_option(int	sockfd);
