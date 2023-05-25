#include "ft_irc.hpp"

class	Channel
{
	public:
		Channel(std::string	name);
		Channel();
		~Channel();

		void	addUser(User &_user);
		User	&getUser(std::string _nick);
	private:
		std::string			_name;
		std::map<int, std::string>	_operator;
		std::map<int, std::string>	_users;
};
