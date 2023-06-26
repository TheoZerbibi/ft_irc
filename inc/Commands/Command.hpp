#ifndef COMMAND_HPP
# define COMMAND_HPP

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <utility>

# define MODES "oklit"
# define MODES_WARG "okl"

# define user_id(serv, nick, user) (":" + nick + "!" + user + "@" + serv)

# define RPL_TOPIC(serv, nick, chan, topic) (":" + serv + " 332 " + nick + " " + chan + " :" + topic)

# define RPL_TOPICWHOTIME(serv, user, channel, nick, setat) (":" + serv +  + " 333 " + user + " " + channel + " " + nick + " " + setat)

# define RPL_NAMREPLY(serv, user, symbol, channel, names) (":" + serv + " 353 " + symbol + " " + channel + " :" + names)

#define ERR_NOSUCHNICK(server, user) (":" + server + " 401 " + user + " :No such user")

#define ERR_NOSUCHCHANNEL(server, user, channel) (":" + server + " 403 " + user + " " + channel + " :No such channel")

# define ERR_NEEDMOREPARAMS(serv, nickname, command) (":" + serv + " 461 " + nickname + " " + command + " :Not enough parameters.\r\n")

# define ERR_ALREADYREGISTERED(serv, nickname) (":" + serv + " 462 " + nickname + " :You may not reregister.\r\n")

#define RPL_UMODEIS(server, user, modes) (":" + server + " 221 "+ user + " :+" + modes)
#define RPL_CHANNELMODIS(server, user, channel, modes, modsarg) (":" + server + " 324 " + user + " " + channel + " " + modes + " " + modsarg)

#define ERR_MODEUNKNOWN(server, user, modechar) (":" + server + " 472 " + modechar + " :is unknown mode char to me")

#define ERR_CHANOPRIVSNEEDED(server, user, channel) ( ":" + server + " 482 " + user + " " + channel + " :You'not channel operator")

#define ERR_USERSDONTMATCHVIEW(server, user) (":" + server + " 502 " + user + " :Cant view mode for other users\r\n")
#define ERR_USERSDONTMATCH(server, user) (":" + server + " 502 " + user + " :Cant change mode for other users\r\n")

#define ERR_UMODEUNKNOWNFLAG(server, user) (":" + server + " 501 " + user + " :Unkown MODE flag")

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
	virtual void execute(int fds, Client *client) = 0;
	
	std::vector<std::string> splitStr(const std::string &cmd, const char sep);
	std::vector<std::string> splitArguments(const std::string &cmd);
};



#endif
