#ifndef TOPICCOMMAND_HPP
# define TOPICCOMMAND_HPP

# include "Command.hpp"
# include "User.hpp"

class TopicCommand : public Command
{
	private:
		std::string _name;
	public:
		TopicCommand();
		virtual ~TopicCommand();
		void execute(int fds, Client *client);
};

#endif
