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
#include	<fstream>


#include	<vector>
#include	<map>
#include	<deque>
#include	<sys/select.h>

#include	"User.hpp"
#include	"Irc.hpp"
#include	"Channel.hpp"
#include	"Reply.hpp"

#define 	SUCCESS 1
#define		FAILURE 0

#define		SEND_BUFF_LEN 1024

# define user_id(serv, nick, user) (":" + nick + "!" + user + "@" + serv)


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

// Signal management
void	handleSigINT(int signal);

struct	addrinfo *setup_addrinfo(char *port);
