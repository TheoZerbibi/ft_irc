#ifndef AWAYCOMMAND_HPP
# define AWAYCOMMAND_HPP
# include "Command.hpp"

# include "User.hpp"

class AwayCommand : public Command
{
private:
	std::string _name;
public:
	AwayCommand();
	virtual	~AwayCommand();
	void	execute(int fds, Client *client);
	bool	cantExecute(Client *client);
};

#endif
