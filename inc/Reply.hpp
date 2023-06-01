#ifndef REPLY_HPP
# define REPLY_HPP

# include <iostream>

class Reply
{
	public:
		Reply(int clientFd, std::string message);
		~Reply(void);

		int			const getClientFd(void) const;
		std::string		getMessage(void) const;

	private:
		int			send();
		std::string		_message;
		int			_clientFd;
};

#endif
