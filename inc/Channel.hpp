#pragma once
#include "ft_irc.hpp"
#include "User.hpp"

#define RPL_PART(userID, channel, reason) (userID + " PART " + channel + " :" + reason + "\r\n")

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

		std::string const	getUsersNick()			const;
		std::string	const	&getTopic()				const;
		std::string	const	&getKey()				const;
		std::string	const	getMemberList()			const;
		int			const	&getLimit()				const;
		char		const	&getType()				const;
		bool		const	&topicIsRestricted()	const;
		bool		const	&isInvit()				const;
		bool					isEmpty() const;
		bool					noOper() const;

		//setter
		void		addUser(User *user);
		void		addOper(User *user);
		void		removeUser(User *user, std::string reason = ".");
		void		removeUser(std::string nick, std::string reason = ".");
		void		removeOper(User *user, std::string reason = ".");
		void		removeOper(std::string nick, std::string reason = ".");

		void		fillOperPos();

		void		setTopic(std::string topic);

		void		setKey(std::string key);
		void		setMaxUser(int value);
		void		setOper(std::string nick, bool value);
		void		setTopicMode(bool value);
		void		setInvit(bool value);

		void		sendToChannel(User *user, std::string msg);

		//Printer
		void		printUserList();

	private:
		std::map<std::string, User *>	_operator;
		std::map<std::string, User *>	_users;

		std::string			_topic;
		std::string			_name;
		std::string			_key;
		char				_type;
		bool				_topicIsOpOnly;
		bool				_isInvit;
		int					_maxUser;
};
