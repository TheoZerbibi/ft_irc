#ifndef QUITCOMMAND_HPP
# define QUITCOMMAND_HPP

# include "Command.hpp"
# include "User.hpp"

class QuitCommand : public Command
{
private:
	std::string _name;
public:
	QuitCommand();
	virtual ~QuitCommand();
	void execute(int fds, Client *client);
	bool cantExecute(Client *client);
};

#endif
