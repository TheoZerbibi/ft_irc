#ifndef COMMAND_HPP
# define COMMAND_HPP

# include <iostream>
# include <string>
# include <string>
# include <map>
# include <utility>

class Client;

class Command
{
protected:
	std::string _name;

public:

	Command();
	virtual ~Command();
	virtual void execute(int fds, Client *client) = 0;
};
#endif
