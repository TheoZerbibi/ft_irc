#ifndef PRIVMSGCOMMAND_HPP
# define PRIVMSGCOMMAND_HPP

# include "Command.hpp"
# include "User.hpp"

class PrivMsgCommand : public Command
{
private:
	void	removeDuplicate(std::vector<std::string> &entry);
	void	sendAllMsg(User *user, std::vector<std::string> targets, std::string msg);
	void	sendMsg(User *user, std::string target, std::string msg);
	std::string _name;
public:
	PrivMsgCommand();
	virtual ~PrivMsgCommand();
	void execute(int fds, Client *client);
	bool cantExecute(Client *client);
};

#endif
