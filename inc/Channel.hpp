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

		//setter
		void		addUser(User *user);
		void		addOper(User *user);
		void		setTopic(std::string topic);
		void		setKey(std::string key);

		int		setMode(int mode, int modemod, User *user, std::vector<std::string> &value);
	private:
		std::map<std::string, User *>	_operator;
		std::map<std::string, User *>	_users;


		int				setKey(int mod, User *user, std::string	*value);
		int				setMaxUser(int mod, User *user, std::string *value);
		int				setOper(int mod, User *ser, std::string *value);
		int				setTopicMode(int mod, User *user);
		int				setInvis(int mod, User *user);



		std::string			_topic;
		std::string			_name;
		std::string			_key;
		bool				_topicIsOpOnly;
		bool				_isInvis;
		int				_maxUser;
};
