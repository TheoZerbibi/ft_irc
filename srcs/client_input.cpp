#include "ft_irc.hpp"

int	Irc::accept_client()
{
	int	newfd;

	if (FD_ISSET(serv->getSocket(), &(this->_fds[READ])))
	{
		std::cout << "Accepting new connection" << std::endl;
		newfd = accept(this->getSocket(), NULL, NULL);
		if (newfd == -1)
			throw SyscallError();
		this->addUser(newfd);
		FD_SET(newfd, &(this->fds[MASTER]));
		return (1);
	}
	return (0);
}

int	Irc::receive_client_data(Client &user)
{
		int	fd;

		fd = user.getFd();
		std::cout << "Got some new message" << std::endl;
		if (FD_ISSET(fd, &fds[READ]))
		{
			if (user.recvData() <= 0)
			{
				FD_CLR(fd, &fds[MASTER]);
				// Need to remove client from client list
				return (0);
			}
		}
		return (1);
}

int	Irc::data_reception_handler()
{
	std::map<int, User>::const_iterator	beg = this->getUsers().begin();
	std::map<int, User>::const_iterator	end = this->getUsers().end();
	int	fd;

	while (beg != end)
	{
		this->read_client_socket(*beg);
		beg++;
	}
	return (0);
}

// Need to change iteration to take userlist instead of an index
int	Irc::manage_incoming_connection()
{
	std::cout << "Got some fd ready for reading" << std::endl;
	this->accept_client();
	this->data_reception_handler();
	return (0);
}
