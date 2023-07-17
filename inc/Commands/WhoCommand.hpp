#ifndef WHOCOMMAND_HPP
# define WHOCOMMAND_HPP

# include "Command.hpp"
# include "User.hpp"

class WhoCommand : public Command
{
	private:
		std::string _name;
	public:
		WhoCommand();
		virtual ~WhoCommand();
		void	execute(int fds, Client *client);
		bool	cantExecute(Client *client);
};

#endif
