#ifndef JOINCOMMAND_HPP
# define JOINCOMMAND_HPP

# include "Command.hpp"
# include "User.hpp"

class JoinCommand : public Command
{
private:
	std::string _name;

	bool	_isJoinable(const std::string &channel);
	bool	_parseArguments(std::string str);
public:
	JoinCommand();
	virtual ~JoinCommand();
	void execute(int fds, Client *client);
};

#endif
