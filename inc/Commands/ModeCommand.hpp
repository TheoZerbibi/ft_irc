#ifndef MODECOMMAND_HPP
# define MODECOMMAND_HPP

# include "Command.hpp"

class ModeCommand : public Command
{
private:
	std::string _name;
public:
	ModeCommand();
	virtual ~ModeCommand();
	void execute(int fds, Client *client);
};

#endif
