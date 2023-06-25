#ifndef JOINCOMMAND_HPP
# define JOINCOMMAND_HPP

# include "Command.hpp"
# include "User.hpp"
# include "Channel.hpp"


class User;
class Channel;

class JoinCommand : public Command
{
	private:
		std::string _name;

		bool	_isJoinable(const std::string &channel);
		bool	_parseArguments(std::string str);
		void	joinChannels(int fds, User *user, std::vector<std::string> &channels, std::vector<std::string> *chans_key);
	public:
		JoinCommand();
		virtual ~JoinCommand();
		void execute(int fds, Client *client);

		//	void	joinChannels(int fds, User &user, std::string &channels, std::string &chans_key);
		void	replyToSuccefulJoin(int fds, User &user, Channel &chan);
};

#endif
