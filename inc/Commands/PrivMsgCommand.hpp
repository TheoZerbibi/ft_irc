#ifndef PRIVMSGCOMMAND_HPP
# define PRIVMSGCOMMAND_HPP

# include "Command.hpp"

class PrivMsgCommand : public Command
{
private:
	std::string _name;
public:
	PrivMsgCommand();
	virtual ~PrivMsgCommand();
	void execute(int fds, Client *client);
};

#endif
