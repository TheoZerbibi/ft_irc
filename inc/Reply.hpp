#ifndef REPLY_HPP
# define REPLY_HPP

# include "ft_irc.hpp"


class Reply
{
	public:
		Reply(int clientFd, std::string message);
		~Reply(void);

		int			getClientFd(void) const;
		std::string		getMessage(void) const;
		int			send();

	private:
		std::string		_message;
		int			_clientFd;
};

#endif
