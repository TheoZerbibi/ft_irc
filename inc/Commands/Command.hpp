#ifndef COMMAND_HPP
# define COMMAND_HPP

# include <iostream>
# include <string>
# include <string>
# include <map>
# include <utility>

# define ERR_NEEDMOREPARAMS(nickname, command) (":irc.localhost 461 " + nickname + " " + command + " :Not enough parameters.\r\n")
# define ERR_ALREADYREGISTERED(nickname) (":irc.localhost 462 " + nickname + " :You may not reregister.\r\n")


class Client;
class Reply;

class Command
{
protected:
	std::string _name;

public:

	Command();
	virtual ~Command();
	virtual void execute(int fds, Client *client) = 0;
};
#endif
