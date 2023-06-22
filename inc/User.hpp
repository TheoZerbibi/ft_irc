#pragma once
#include "ft_irc.hpp"

#define	DELIM "\r\n"

# define RPL_WELCOME(server, nickname) (":" + server + " 001 " + nickname + " :Welcome to the Internet Relay Network\r\n")
# define RPL_YOURHOST(server,nickname) (":" + server + " 002 " + nickname + " :Your host is " + server + " (localhost)\r\n")
# define RPL_INFO(server,nickname) (":" + server + " 003 " + nickname + " :This server was created by iguidado & thzeribi.\r\n")

class Irc;
class Channel;

class Client {
	public:
		Client(int const &sfd);
		Client(Client *cpy);
		virtual ~Client();

		const std::string	&getNick() const;
		const std::string	&getHost() const;
		const std::string	&getUser() const;
		const std::string	&getRealname() const;

		const int		&getSockfd() const;
		const std::string	&getBuff() const;

		void	setNick(std::string nick);
		void	setHost(std::string host);
		void	setUser(std::string user);
		void	setRealname(std::string realname);

		bool	recvData();

		std::deque<std::string>	&getCmds();

		//		Authentification process
		void		setAuth(bool auth);
		bool const	&isAuth() const;

		//		Registration process : need to implement registrement checking
		bool const	&isRegistered() const;
		void		readyToRegister();

	protected:
		Client();

		// User personnal socket
		std::string			_buff;
		std::deque<std::string>		_cmds;

		int			_sockFd;
		bool		_isAuth;
		bool		_registered;
  
		// Needed to register users
		std::string	_nickname;   
		std::string	_hostname;
		std::string	_username;
		std::string	_realname;

		void					extractCmds();
		void					printCmds();
//	private:
	//	int		connection_time; //if current_time - connection_time > 2s timeout client
};

class User : public Client{
	public:
//		User(int fd, const std::string& nick, const std::string& username, const std::string& hostname);
//		User(int &fd);
		User(Client *client);
		virtual ~User();

		//	const int &getFd() const; // Other methods and data members as needed

		bool	isOper() const;
		bool	isInvis() const;
		bool	isChannelOper(Channel *chan) const;
	private:
		User();
		std::vector<Channel *>	_chans;
		bool			_isOper;
		bool			_isInvis;
};

// User 
bool	ft_check_client_registration(Client &client);
