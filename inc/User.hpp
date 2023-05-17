#pragma once
#include <string>

class User {
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
