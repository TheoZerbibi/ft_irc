#pragma once
#include "ft_irc.hpp"
#include "User.hpp"

class User;

class	Channel
{
	public:
		Channel(std::string	name, std::string key = "", bool topicIsOpOnly = false, int maxUser = 0);
		~Channel();

		void	addUser(User &_user);
		User	&getUser(std::string _nick);
	private:
		std::map<std::string, User *>	_operator;
		std::map<std::string, User *>	_users;

		std::string			_name;
		std::string			_key;
		bool				_topicIsOpOnly;
		int					_maxUser;
};
