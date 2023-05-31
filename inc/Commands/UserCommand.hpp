#ifndef USERCOMMAND_HPP
# define USERCOMMAND_HPP

# include "Command.hpp"
# include "User.hpp"

class UserCommand : public Command
{
private:
	std::string _name;
public:
	UserCommand();
	virtual ~UserCommand();
	void execute(int fds, Client *client);
};

#endif
