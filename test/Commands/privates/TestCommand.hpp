#ifndef TESTCOMMAND_HPP
# define TESTCOMMAND_HPP

# include "Command.hpp"

class TestCommand : public Command
{
private:
	std::string _name;
public:
	TestCommand();
	virtual ~TestCommand();
	void execute();
};


#endif
