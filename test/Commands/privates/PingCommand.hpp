#ifndef PINGCOMMAND_HPP
# define PINGCOMMAND_HPP
# include "Command.hpp"

class PingCommand : public Command
{
private:
	std::string _name;
public:
	PingCommand();
	virtual ~PingCommand();
	void execute();
};


#endif
