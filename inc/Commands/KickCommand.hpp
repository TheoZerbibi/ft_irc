#ifndef KICKCOMMAND_HPP
# define KICKCOMMAND_HPP

# include "Command.hpp"
# include "User.hpp"

class KickCommand : public Command
{
private:
	std::string _name;
public:
	KickCommand();
	virtual ~KickCommand();
	void execute(int fds, Client *client);
};


#endif
