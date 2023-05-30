#ifndef COMMAND_HPP
# define COMMAND_HPP

# include <iostream>
# include <string>
# include <string>
# include <map>
# include <utility>

class Command
{
protected:
	std::string _name;

public:

	Command();
	virtual ~Command();
	virtual void execute() = 0;
};

std::map<std::string, Command*> *getCommandList();
#endif
