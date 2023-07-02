#ifndef NICKCOMMAND_HPP
# define NICKCOMMAND_HPP

# include "Command.hpp"
# include "User.hpp"

# define ERR_NICKNAMEINUSE(serv, client, nickname) (":" + serv + " 433 " + client + " " + nickname + " ::Nickname is already in use")


class NickCommand : public Command
{
private:
	std::string _name;
public:
	NickCommand();
	virtual ~NickCommand();
	void execute(int fds, Client *client);
	bool cantExecute(Client *client);
};

#endif
