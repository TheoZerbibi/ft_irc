#pragma once
#include "ft_irc.hpp"

#define	DELIM "\r\n"

# define RPL_WELCOME(server, user, userID) (":" + server + " 001 " + user + " :Welcome to the Internet Relay Network " + userID + "\r\n")
# define RPL_YOURHOST(server,nickname) (":" + server + " 002 " + nickname + " :Your host is " + server + " (localhost)\r\n")
# define RPL_INFO(server,nickname) (":" + server + " 003 " + nickname + " :This server was created by iguidado & thzeribi.\r\n")
# define RPL_INVITELIST(server, user, channel) (":" + server + " 336 " + user + " " + channel)
# define RPL_ENDOFINVITELIST(server, client) (":" + server + " 337 " + client + " :End of /INVITE list")

class Irc;
class Channel;

class Client {
	public:
		Client(int const &sfd);
		Client(Client *cpy);
		virtual ~Client();

		//Getter
		const std::string	&getNick() const;
		const std::string	&getHost() const;
		const std::string	&getUser() const;
		const std::string	&getRealname() const;
		const int		&getSockfd() const;
		const std::string	&getBuff() const;
		
		//Setter
		void		setNick(std::string nick);
		void		setHost(std::string host);
		void		setUser(std::string user);
		void		setRealname(std::string realname);

		bool			recvData();

		std::deque<std::string>	&getCmds();

		//		Authentification process
		void		setAuth(bool auth);
		bool const	&isAuth() const;

		//		Registration process
		bool const	&isRegistered() const;

	protected:
		Client();

		// User personnal socket
		std::string			_buff;
		std::deque<std::string>		_cmds;

		int			_sockFd;
		bool			_isAuth;
		bool			_registered;
  
		// Needed to register users
		std::string	_nickname;   
		std::string	_hostname;
		std::string	_username;
		std::string	_realname;

		void		extractCmds();
		void		printCmds();
//	private:
	//	int		connection_time; //if current_time - connection_time > 2s timeout client
};

class User : public Client{
	public:
//		User(int fd, const std::string& nick, const std::string& username, const std::string& hostname);
//		User(int &fd);
		User(Client *client);
		virtual ~User();

		//getter
		bool	isOper() const;
		bool	isInvis() const;
		bool	isChannelOper(Channel *chan) const;
		bool	isInvited(Channel *chan);


		// Channel management
		void					addChannel(Channel *chan);
		void					removeChannel(Channel *chan);
		void					joinChannel(std::string channame, std::string key);
		void					quitChannel(Channel *chan, std::string const &msg);
		void					quitAllChannel(std::string const &msg = "Leaving.");
		std::vector<Channel *>::iterator	getChannel(Channel *chan);
		void					inviteOnChannel(Channel *chan);
		void					removeInvite(Channel *chan);

		//Utils
		void	printInvited();

		bool		isOnChannel(Channel *chan);

		void	setInvis(bool status);
		void	setOper(bool status);
		void	printInfo();
	private:
		User();
		std::vector<Channel *>	_chans;
		std::vector<Channel *>	_invited;
		bool			_isOper;
		bool			_isInvis;
		bool			_isChannelUser(Channel *chan) const;

		//printer
		void	printChannels();

};

// User 
bool	ft_check_client_registration(Client &client);
