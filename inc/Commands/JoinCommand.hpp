#ifndef JOINCOMMAND_HPP
# define JOINCOMMAND_HPP

# include "Command.hpp"
# include "User.hpp"

# define RPL_

class User;

class JoinCommand : public Command
{
private:
	std::string _name;

	bool	_isJoinable(const std::string &channel);
	bool	_parseArguments(std::string str);
	void	joinChannels(int fds, User *user, std::vector<std::string> &channels, std::vector<std::string> *chans_key);
public:
	JoinCommand();
	virtual ~JoinCommand();
	void execute(int fds, Client *client);
};

#endif
