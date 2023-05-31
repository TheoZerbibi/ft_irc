#ifndef NICKCOMMAND_HPP
# define NICKCOMMAND_HPP

# include "Command.hpp"
# include "User.hpp"

class NickCommand : public Command
{
private:
	std::string _name;
public:
	NickCommand();
	virtual ~NickCommand();
	void execute(int fds, Client *client);
};

#endif
