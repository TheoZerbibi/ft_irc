#ifndef JOINCOMMAND_HPP
# define JOINCOMMAND_HPP

# include "Command.hpp"

class JoinCommand : public Command
{
private:
	std::string _name;
public:
	JoinCommand();
	virtual ~JoinCommand();
	void execute();
};

#endif
