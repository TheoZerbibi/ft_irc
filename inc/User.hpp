#pragma once
#include "ft_irc.hpp"

class Irc;
class Channel;

class Client {
	public:
		Client(int const &sfd);
		virtual ~Client();

		const std::string	&getNick() const;
		const std::string	&getHost() const;
		const std::string	&getUser() const;
		const int		&getSockfd() const;

		void	setNick(std::string nick);
		void	setHost(std::string host);
		void	setUser(std::string user);

		bool		   recvData();

		//		Registration process : need to implement registrement checking
		bool const	&isRegistered() const;

	protected:
		Client();

		// User personnal socket
		std::string			_buff;
		std::deque<std::string>	_cmds;

		int		_sockFd;
		bool		_registered;

		// Needed to register users
		std::string	_nickname;   
		std::string	_hostname;
		std::string	_username;

		void		exctractCmds();
	private:
	//	int		connection_time; //if current_time - connection_time > 2s timeout client
};

class User : public Client{
	public:
//		User(int fd, const std::string& nick, const std::string& username, const std::string& hostname);
//		User(int &fd);
		User(Client &client);
		virtual ~User();

		//	const int &getFd() const; // Other methods and data members as needed

	private:
		User();
		std::vector<std::string>	channel;
		// Other data members as needed
		//	int		mode;
};

// User 
bool	ft_check_client_registration(Client &client);
