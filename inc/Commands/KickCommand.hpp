#ifndef KICKCOMMAND_HPP
# define KICKCOMMAND_HPP

# include "Command.hpp"

class KickCommand : public Command
{
private:
	std::string _name;
public:
	KickCommand();
	virtual ~KickCommand();
	void execute();
};


#endif
