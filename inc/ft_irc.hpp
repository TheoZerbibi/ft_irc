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
#include	"Irc.hpp"


#define 	SUCCESS 1
#define		FAILURE 0


class	SyscallError: public std::exception
{
	const char *what() const throw(){
		return (strerror(errno));
	}
};

// select() argument type : see fd_set
enum	e_fd_triggers
{
	MASTER,
	READ,
	SEND,
	EXCEPT,
	FD_SET_TYPES
};

class	Channel
{
	public:
		Channel(std::string	name);
		~Channel();

		void	addUser(User &_user);
		User	&getUser(std::string _nick);
	private:
		Channel();
		std::string			_name;
		std::map<int, std::string>	_operator;
		std::map<int, std::string>	_users;
};



// Signal management
void	handleSigINT(int signal);

struct	addrinfo *setup_addrinfo(char *port);
