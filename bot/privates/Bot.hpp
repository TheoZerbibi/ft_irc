#ifndef BOT_HPP
# define BOT_HPP
# include <iostream>
# include <string>
# include <cstdlib>
# include <sys/types.h>
# include <sys/socket.h>
# include <signal.h>

#include "Socket.hpp"

class Bot
{
	public:
		Bot(std::string name);
		~Bot(void);

		void		setName(std::string name);
		void		setHost(std::string host);
		void		setRealname(std::string realname);
		void		setUser(std::string user);
		void		setPass(std::string pass);
		void		setPort(int port);

		std::string	&getName(void) const;
		std::string	&getHost(void) const;
		std::string	&getRealname(void) const;
		std::string	&getUser(void) const;
		std::string	&getPass(void) const;
		int			&getPort(void) const;

		void		setupSignal(void);
		void		quit(int signal);
		void		connect(std::string host, int port, std::string password);
		void		run(void);

	private:
		struct sigaction	_sa;
		Sock				_socket;
		std::string			_name;
		std::string			_host;
		std::string			_realname;
		std::string			_user;
		std::string			_pass;
		int					_port;

		Bot(void);

};

#endif
