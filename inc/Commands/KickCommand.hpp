#ifndef KICKCOMMAND_HPP
# define KICKCOMMAND_HPP

# include "Command.hpp"
# include "User.hpp"
# include "Channel.hpp"

class KickCommand : public Command
{
private:
	std::string _name;

		std::map<Channel *, User *>			_parseArg(Client *executor, std::vector<std::string> &args);
		void								_kickUser(Client *executor, std::map<Channel *, User *> kicks, std::string reason);
public:
	KickCommand();
	virtual ~KickCommand();
	void execute(int fds, Client *client);
	bool cantExecute(Client *client);
};


#endif
