#ifndef SOCK_H
# define SOCK_H

# include <unistd.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <string.h>
# include <iostream>

class Sock {
private:
	int		_fd;
	int		_bi;
	char	_buffer[512];
public:
	Sock();
	int	getFd() const;
	int	link(const char* host, int port);
	int	unlink();
	int	in(char* buf);
	int	out(const char* buf);
	
	Sock&	operator <<(const std::string &msg);
};

#endif
