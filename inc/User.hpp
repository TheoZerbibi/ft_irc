#pragma once
#include <string>

class Client {
	public:
		Client(int &sfd);
		~Client();
		Client();

		
		const std::string	&getNick(std::string nick) const;
		const std::string	&getHost(std::string Host) const;
		const std::string	&getUser(std::string User) const;
		
		void	setNick(std::string nick);
		void	setHost(std::string host);
		void	setUser(std::string user);
	private:

		int		_sockFd;
		std::string	_nickname;
		std::string	_hostname;
		std::string	_username;
};

bool	ft_check_client_registration(Client &client);


class User : public Client{
	public:

//		User(int fd, const std::string& nick, const std::string& username, const std::string& hostname);
//		User(int &fd);
		User();
		~User();

	//	const int &getFd() const;
		const std::string& getNick() const;
		const std::string& getUsername() const;
		const std::string& getHostname() const;
		void		   recvData();

		// Other methods and data members as needed

	private:
	//	int _fd;                 // User's file descriptor
		std::string _cmdbuffer;
		std::string _nick;       // User's nickname
		std::string _username;   // User's username
		std::string _hostname;   // User's hostname or IP address

		// Other data members as needed
};
