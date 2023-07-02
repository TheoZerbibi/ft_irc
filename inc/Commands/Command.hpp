#ifndef COMMAND_HPP
# define COMMAND_HPP

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <utility>
//# include "Reply.hpp"

# define user_id(serv, nick, user) (":" + nick + "!" + user + "@" + serv)
# define user_ids(serv, nick, user) (nick + "!" + user + "@" + serv)

#define REMOVING 0
#define ADDING 1

class Client;
class User;
class Reply;
class Irc;

class Command
{
	protected:
		std::string _name;

	public:

		Command();
		virtual ~Command();
		virtual void	execute(int fds, Client *client) = 0;
		virtual bool	cantExecute(Client *client);

		std::vector<std::string> splitStr(const std::string &cmd, const char sep);
		std::vector<std::string> splitArguments(const std::string &cmd);
};


#endif
