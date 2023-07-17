#ifndef REPLY_HPP
# define REPLY_HPP

# include "ft_irc.hpp"


# define RPL_NAMREPLY(server, client, symbol, channel, memberList) (":" + server + " 353 " + client + " " + symbol + " " + channel + " :" + memberList + "\r\n")
#define RPL_STARTOFMOTD(server, nickname, message) (":" + server + " 375 " + nickname + " :- " + message + " Message of the day - \r\n")
#define RPL_MOTD(server, nickname, motdLine) (":" + server + " 372 " + nickname + " :" + motdLine + "\r\n")
#define RPL_ENDOFMOTD(server, nickname) (":" + server + " 376 " + nickname + " :End of /MOTD command.\r\n")

# define RPL_UMODEIS(server, user, modes) (":" + server + " 221 "+ user + " :+" + modes + "\r\n")
# define RPL_CHANNELMODIS(server, user, channel, modes, modsarg) (":" + server + " 324 " + user + " " + channel + " " + modes + " " + modsarg + "\r\n")
# define RPL_NOTOPIC(server, user, channel) (":" + server + " 331 " + user + " " + channel + " :No topic is set\r\n")
# define RPL_TOPICWHOTIME(serv, user, channel, nick, setat) (":" + serv +  + " 333 " + user + " " + channel + " " + nick + " " + setat + "\r\n")
# define RPL_TOPIC(serv, nick, chan, topic) (":" + serv + " 332 " + nick + " " + chan + " :" + topic + "\r\n")
# define RPL_ENDOFNAMES(server, user)  (":" + server + " 366 " + user + " :End of /NAMES list" + "\r\n")
# define RPL_QUIT(userID, reason) (userID + " QUIT :Quit: " + reason + "\r\n")
# define RPL_PRIVMSG(userID, target, msg) (userID + " PRIVMSG " + target + " :" + msg + "\r\n")
# define RPL_KICK(userID, channel, target, reason) (userID + " KICK " + channel + " " + target + " " + reason + "\r\n")

#define ERR_NOMOTD(server, nickname) (":" + server + " 422 " + nickname + " :MOTD File is missing\r\n")
# define ERR_NOSUCHCHANNEL(server, nickname, channel) (": " + server + " 403 " + nickname + " " + channel + " :No such channel\r\n")
# define ERR_NOTONCHANNEL(server, nickname, channel) (": " + server + " 442 " + nickname + " " + channel + " :The user is not on this channel.\r\n")
# define ERR_CANNOTSENDTOCHAN(server, nickname, channel) (":" + server + " 404 " + nickname + " " + channel + " :Cannot send to channel\r\n")
# define ERR_USERONCHANNEL(server, user, channel) (":" + server + " 443 " + user + " " + channel + " :is already on channel.\r\n")

# define ERR_NOSUCHNICK(server, user) (":" + server + " 401 " + user + " :No such user\r\n")
# define ERR_ALREADYREGISTERED(serv, nickname) (":" + serv + " 462 " + nickname + " :You may not reregister.\r\n")

# define ERR_NEEDMOREPARAMS(serv, nickname, command) (":" + serv + " 461 " + nickname + " " + command + " :Not enough parameters.\r\n")

#define ERR_MODEUNKNOWN(server, user, modechar) (":" + server + " 472 " + modechar + " :is unknown mode char to me.\r\n")
#define ERR_INVITEONLYCHAN(server, user, channel) ( ":" + server + " 473 " + user + " " + channel + " :Cannot join channel (+i).\r\n")
#define ERR_CHANOPRIVSNEEDED(server, user, channel) ( ":" + server + " 482 " + user + " " + channel + " :You'not channel operator.\r\n")

#define ERR_USERSDONTMATCHVIEW(server, user) (":" + server + " 502 " + user + " :Cant view mode for other users\r\n")
#define ERR_USERSDONTMATCH(server, user) (":" + server + " 502 " + user + " :Cant change mode for other users\r\n")

#define ERR_UMODEUNKNOWNFLAG(server, user) (":" + server + " 501 " + user + " :Unkown MODE flag\r\n")


class Reply
{
	public:
		Reply(int clientFd = 0, std::string message = "");
		~Reply(void);

		int			getClientFd(void) const;
		std::string	const	&getMessage(void) const;
		int			send();

		void			setMessage(std::string mess);

		Reply	&operator=(Reply const &rhs);
		Reply	&operator+=(Reply const &rhs);
	private:
		std::string		_message;
		int			_clientFd;
};

#endif
