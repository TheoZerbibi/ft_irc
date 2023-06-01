#ifndef PASSCOMMAND_HPP
# define PASSCOMMAND_HPP
# include "Command.hpp"

# include "User.hpp"

class PassCommand : public Command
{
private:
	std::string _name;
public:
	PassCommand();
	virtual ~PassCommand();
	void execute(int fds, Client *client);
};

#endif
