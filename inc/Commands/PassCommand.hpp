#ifndef PASSCOMMAND_HPP
# define PASSCOMMAND_HPP
# include "Command.hpp"
# include "User.hpp"
# include "Reply.hpp"

# define ERR_PASSWDMISMATCH(serv, nickname) (":" + serv + " 464 " + nickname + " :Password incorrect.\r\n")

class PassCommand : public Command
{
private:
	std::string _name;
public:
	PassCommand();
	virtual ~PassCommand();
	void execute(int fds, Client *client);
	bool cantExecute(Client *client);
};

#endif
