#ifndef INVITECOMMAND_HPP
# define INVITECOMMAND_HPP

# include "Command.hpp"

class InviteCommand : public Command
{
private:
	std::string _name;
public:
	InviteCommand();
	virtual ~InviteCommand();
	void execute();
};

#endif
