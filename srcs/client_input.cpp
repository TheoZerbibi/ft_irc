#include "ft_irc.hpp"

int	Irc::accept_client()
{
	int	newfd;

	if (FD_ISSET(this->getSocket(), &(this->fds[READ])))
	{
		newfd = accept(this->getSocket(), NULL, NULL);
		if (newfd == -1)
			throw SyscallError();
		this->addClient(newfd);
		FD_SET(newfd, &(this->fds[MASTER]));
		std::cout << "Accepting new connection" << std::endl;
		return (1);
	}
	return (0);
}

int	Irc::receive_client_data(Client *user)
{
		int	fd;

		fd = user->getSockfd();
		std::cout << "testing new message ..." << std::endl;
		if (FD_ISSET(fd, &fds[READ]))
		{
			std::cout << "Got some new message" << std::endl;
			if (user->recvData() <= 0)
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
	std::map<int, Client*>::iterator	beg = this->getClients()->begin();
	std::map<int, Client*>::iterator	end = this->getClients()->end();

//	std::map<int, User>::iterator	beg = this->getUsers().begin();
//	std::map<int, User>::iterator	end = this->getUsers().end();

	while (beg != end)
	{
		this->receive_client_data(beg->second);
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
