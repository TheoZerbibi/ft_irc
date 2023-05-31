#ifndef PARTCOMMAND_HPP
# define PARTCOMMAND_HPP

# include "Command.hpp"
# include "User.hpp"

class PartCommand : public Command
{
private:
	std::string _name;
public:
	PartCommand();
	virtual ~PartCommand();
	void execute(int fds, Client *client);
};

#endif
