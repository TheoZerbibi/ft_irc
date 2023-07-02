#ifndef INVITECOMMAND_HPP
# define INVITECOMMAND_HPP

# include "Command.hpp"
# include "User.hpp"

# define RPL_INVITE(userID, target, channel) (userID + " INVITE " + target + " " + channel + "\r\n")
# define RPL_INVITING(server, client, nick, channel) (":" + server + " 341 " + client + " " + nick + " " + channel + "\r\n")

class InviteCommand : public Command
{
	private:
		std::string _name;
	public:
		InviteCommand();
		virtual ~InviteCommand();
		void execute(int fds, Client *client);
		bool cantExecute(Client *client);
};

#endif
