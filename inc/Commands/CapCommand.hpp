#ifndef CAPCOMMAND_HPP
# define CAPCOMMAND_HPP
# include "Command.hpp"

class CapCommand : public Command
{
private:
	std::string _name;
public:
	CapCommand();
	virtual ~CapCommand();
	void execute(int fds, Client *client);
	bool cantExecute(Client *client);
};

#endif
