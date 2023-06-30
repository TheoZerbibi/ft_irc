#ifndef MODECOMMAND_HPP
# define MODECOMMAND_HPP

# include "Command.hpp"
# include "User.hpp"
# include "Channel.hpp"
# include <sstream>

# define MODES "oklit"
# define MODES_WARG "okl"


#define RPL_UMODEIS(server, user, modes) (":" + server + " 221 "+ user + " :+" + modes)
#define RPL_CHANNELMODIS(server, user, channel, modes, modsarg) (":" + server + " 324 " + user + " " + channel + " " + modes + " " + modsarg)

#define ERR_MODEUNKNOWN(server, user, modechar) (":" + server + " 472 " + modechar + " :is unknown mode char to me")

#define ERR_CHANOPRIVSNEEDED(server, user, channel) ( ":" + server + " 482 " + user + " " + channel + " :You'not channel operator")

#define ERR_UMODEUNKNOWNFLAG(server, user) (":" + server + " 501 " + user + " :Unkown MODE flag")

#define ERR_USERSDONTMATCHVIEW(server, user) (":" + server + " 502 " + user + " :Cant view mode for other users\r\n")
#define ERR_USERSDONTMATCH(server, user) (":" + server + " 502 " + user + " :Cant change mode for other users\r\n")



//#define ERR_INVALIDMODEPARAM(server, user,)


#define REMOVING 0
#define ADDING 1


class ModeCommand : public Command
{
	private:
		std::string _name;

	//	void	executeUserMode(int fds, User *user, std::vector<std::string> &args);
	//	void	printUserMode(int fds, User *user, std::string nickname);
	//	void	applyUserMode(int fds, User *user, Client *target, std::string &modstr, std::vector<std::string> *modvalue);
		void	applyChannelMode(User *user, Channel *chan, std::string &modstr, std::vector<std::string> &modvalue);
		void	appChannelMode(char mode, int modmode, Channel *chan, std::string *arg);
		void	executeChannelMode(int fds, User *user, std::vector<std::string> &args);
		int	checkOperMode(int fds, User *user, std::string &target);
		void	printChannelMode(int fds, User *user, Channel *chan);
		bool			is_mode(char c);
		bool			needArg(char mode, bool modmode);
		int			checkMode(char mode, bool modmode, User *user, Channel *chan, std::vector<std::string> &args);

		void	printUserMode(int fds, User *user, std::string nickname);
		void	executeUserMode(int fds, User *user, std::vector<std::string> &args);
	public:
		ModeCommand();
		virtual ~ModeCommand();
		virtual void execute(int fds, Client *client);
};


#endif
