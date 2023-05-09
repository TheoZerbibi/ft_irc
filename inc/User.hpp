class User {
	public:
		User(int fd, const std::string& nick, const std::string& username, const std::string& hostname);
		~User();

		int getFd() const;
		const std::string& getNick() const;
		const std::string& getUsername() const;
		const std::string& getHostname() const;
		void		   recvData();

		// Other methods and data members as needed

	private:
		int m_fd;                 // User's file descriptor
		std::string m_cmdbuffer;
		std::string m_nick;       // User's nickname
		std::string m_username;   // User's username
		std::string m_hostname;   // User's hostname or IP address

		// Other data members as needed
};
