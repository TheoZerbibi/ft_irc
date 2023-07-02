#ifndef PINGCOMMAND_HPP
# define PINGCOMMAND_HPP
# include "Command.hpp"

# include "User.hpp"
# define RPL_PONG(userId, token) (userId + " PONG :" + token + "\r\n")

class PingCommand : public Command
{
private:
	std::string _name;
public:
	PingCommand();
	virtual ~PingCommand();
	void execute(int fds, Client *client);
	bool cantExecute(Client *client);
};

#endif
