#pragma once
#include "ft_irc.hpp"
#include "User.hpp"

class User;

class	Channel
{
	public:
		Channel(std::string	name, std::string key = "", bool topicIsOpOnly = false, int maxUser = 0);
		~Channel();

		//getter
		std::string	const &getName() const;
		User		*getOper(std::string nick);
		User		*getUser(std::string nick);

		std::string	const &getTopic() const ;
		std::string	const &getKey() const;
		int		const &getLimit() const;
		bool		const &topicIsRestricted() const;
		bool		const &isInvis() const;

		//setter
		void		addUser(User *user);
		void		addOper(User *user);
		void		removeUser(User *user);
		void		removeUser(std::string nick);
		void		removeOper(User *user);
		void		removeOper(std::string nick);

		void		setTopic(std::string topic);

		void		setKey(std::string key);
		void		setMaxUser(int value);
		void		setOper(std::string nick, bool value);
		void		setTopicMode(bool value);
		void		setInvis(bool value);
	private:
		std::map<std::string, User *>	_operator;
		std::map<std::string, User *>	_users;

		std::string			_topic;
		std::string			_name;
		std::string			_key;
		bool				_topicIsOpOnly;
		bool				_isInvis;
		int				_maxUser;
};
