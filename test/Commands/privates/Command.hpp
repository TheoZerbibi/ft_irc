#ifndef COMMAND_HPP
# define COMMAND_HPP

# include <iostream>
# include <string>

class Command
{
protected:
	std::string _name;

public: 
	Command();
	virtual ~Command();
	virtual void execute() = 0;
};

#endif
