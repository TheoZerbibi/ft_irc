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

		int		   recvData();

	protected:
		int		_sockFd;
		std::string	_cmdbuffer;
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

		// Other methods and data members as needed

	private:

		// Other data members as needed
};
