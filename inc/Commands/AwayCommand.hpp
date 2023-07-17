#ifndef AWAYCOMMAND_HPP
# define AWAYCOMMAND_HPP
# include "Command.hpp"

# include "User.hpp"

# define RPL_NOWAWAY(server, nickname) (":" + server + " 306 " + nickname + " :You have been marked as being away\r\n")
# define RPL_UNAWAY(server, nickname) (":" + server + " 305 " + nickname + " :You are no longer marked as being away\r\n")
# define RPL_AWAY(server, nickname, user, awayMessage) (":" + server + " 301 " + nickname + " " + user + " :" + awayMessage + "\r\n")

class AwayCommand : public Command
{
private:
	std::string _name;
public:
	AwayCommand();
	virtual	~AwayCommand();
	void	execute(int fds, Client *client);
	bool	cantExecute(Client *client);
};

#endif
