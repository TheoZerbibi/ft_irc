#ifndef JOINCOMMAND_HPP
# define JOINCOMMAND_HPP

# include "Command.hpp"
# include "User.hpp"

class JoinCommand : public Command
{
private:
	std::string _name;
public:
	JoinCommand();
	virtual ~JoinCommand();
	void execute(int fds, Client *client);
};

#endif
