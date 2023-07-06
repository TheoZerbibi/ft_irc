#ifndef PARTCOMMAND_HPP
# define PARTCOMMAND_HPP

# include "Command.hpp"
# include "User.hpp"

class PartCommand : public Command
{
private:
	std::string _name;

	std::vector<std::string>	_parseArg(std::vector<std::string> &args);
	void						_leaveChannel(int fds, Client *client, std::vector<std::string> channels, std::string msg);
	bool						_chanIsValid(const std::string &name);

public:
	PartCommand();
	virtual ~PartCommand();
	void execute(int fds, Client *client);
	bool cantExecute(Client *client);
};

#endif
