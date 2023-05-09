#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<netinet/tcp.h>
#include	<arpa/inet.h>

#include	<unistd.h> //need to be deleted and replaced by recv and send

#include	<cstring>
#include	<netdb.h>

#include	<errno.h>

#include	<iostream>
#include	<algorithm>

#include	<vector>

#include	"User.hpp"

class	Irc{
	private:
		int			_sockfd;
		struct	addrinfo	*_net;
//		std::vector<Channel>	chans;
		std::vector<User>	_users;
	public:
		//Const & destr
		Irc();
		Irc(std::string port, std::string passwd);
		~Irc();

		//Utils
		void			printAi() const;

		//Setter
		void			addUser(int const &sfd);

		//Getter
		int			&getSocket() const;
		struct addrinfo		*getAi() const;
		std::vector<User>	&getUsers() const;

		//Operator Overload
		Irc operator=(Irc &rhs);
};

class	SyscallError: public std::exception
{
	const char *what() const throw(){
		return (strerror(errno));
	}
};


void	Irc::addUser(int const &sfd)
{
	std::cout << "size of int ref" << sizeof(sfd) << std::endl;
}

int	ft_setup_socket(struct addrinfo *net);
struct	addrinfo *setup_addrinfo(char *port);
int	set_socket_option(int	sockfd);
