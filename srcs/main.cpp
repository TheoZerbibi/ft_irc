#include "ft_irc.hpp"

int	ft_accept_client(Irc *serv, fd_set *fds)
{
	int	newfd;

	if (FD_ISSET(serv->getSocket(), &fds[READ]))
	{
		std::cout << "Accepting new connection" << std::endl;
		newfd = accept(serv->getSocket(), NULL, NULL);
		if (newfd == -1)
			throw SyscallError();
		FD_SET(newfd, &fds[MASTER]);
		serv->addUser(newfd);
	}
	return (0);
}

int	ft_receive_userdata(Irc *serv, fd_set *fds)
{
	std::map<int, User>::const_iterator	beg = serv->getUsers().begin();
	std::map<int, User>::const_iterator	end = serv->getUsers().end();
	char	disc[512];
	int	nbytes;
	int	fd;

	while (beg != end)
	{
		fd = beg->first;
		std::cout << "Got some new message" << std::endl;
		if (FD_ISSET(fd, &fds[READ]))
		{
			if (beg->second.recvData() <= 0)
			{
				close(fd);
				FD_CLR(fd, &fds[MASTER]);
			}
		}
		beg++;
	}
	return (0);
}

// Need to change iteration to take userlist instead of an index
int	read_fds(Irc *serv, fd_set *fds)
{
	std::cout << "Got some fd ready for reading" << std::endl;
	ft_accept_client(serv, fds);
	ft_receive_userdata(serv, fds);
	return (0);
}

bool	ft_check_input(int ac, char **av)
{
	std::string		port;
	std::string::iterator	beg;
	std::string::iterator	end;

	if (ac != 3)
	{
		std::cout << "Wrong usage: format must be './ircserv <port> <password>'" << std::endl;
		return (1);
	}
	port = av[1];
	beg = port.begin(), end = port.end();
	while (beg != end)
	{
		if (!std::isdigit(*beg))
		{
			std::cout << "Wrong port format: '" << port << "' should only contain digit" << std::endl;
			return (1);
		}
		beg++;
	}
	return (0);
}


int	main_loop(Irc &serv)
{
	fd_set				fds[4];
	int				fdmax;
	//	timeval				ttd = (timeval){2, 0};

	for (int i = 0; i < 4; i++)
		FD_ZERO(&fds[i]);
	FD_SET(serv.getSocket(), &fds[MASTER]);
	while (1)
	{
		for (int i = 1; i < 4; i++)
			fds[i] = fds[MASTER];
		fdmax = serv.computeFdMax();
		std::cout << "Connecting" << std::endl;
		if (select(fdmax, &fds[READ], NULL, NULL, NULL) == -1)
		{
			std::cerr << "Select error : ";
			throw  SyscallError();
		}
		read_fds(&serv, fds);
		std::cout << "Hey you !" << std::endl;
	}
	return (0);
}

int	main(int ac, char **av)
{
	(void)av;
	(void)ac;

	std::signal(SIGINT, handleSigINT);
	try
	{
		Irc	&ircserv = Irc::getInstance();
		ircserv.printAi();
		main_loop(ircserv);
	}
	catch (std::exception &e)
	{
		if (strcmp(e.what(), "Success"))
			std::cerr << e.what() << std::endl;
	}
	return (0);
}
