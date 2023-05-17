#include "ft_irc.hpp"

int	ft_accept_client(Irc *serv, fd_set *fds)
{
	int	newfd;

	newfd = accept(serv->getSocket(), NULL, NULL);
	if (newfd == -1)
		throw SyscallError();
	FD_ISSET(newfd, &fds[MASTER]);
	serv->addUser(newfd);
	return (0);
}

// Need to change interation to take userlist instead of an index
int	read_fds(Irc *serv, fd_set *fds)
{
	std::vector<int>::iterator	beg = serv->_clients.begin();
	std::vector<int>::iterator	end = serv->_clients.end();
	char	disc[512];
	int	fdmax = serv->computeFdMax();
	int	nbytes;

	(void)beg;
	(void)end;
	std::cout << "Got some fd ready for reading" << std::endl;
	for (int i = 0; i <= fdmax; i++)
	{
		if (FD_ISSET(i, &fds[READ]))
		{
			if (i == serv->getSocket())
			{
				std::cout << "Accepting new connection" << std::endl;
				ft_accept_client(serv, fds);
			}
			else if ((nbytes = recv(i, disc, sizeof(disc), 0)) <= 0)
			{
				std::cout << "Error with recv" << std::endl;
//			if (nbytes == 0)
//			{
//			//Connection closed : Need to discard User entry from userlist
//			}
//			else
//			{
//				//Error from recv
//			}
//			close(i);
//			
			}
			else
			{
				std::cout << i << ": " << disc << std::endl;
				bzero(disc, sizeof(disc));
			}
	}
}
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


//	void	ft_read_socket(int sfd)
//	{
//		struct addrinfo net;
//		struct sockaddr	sa;
//	
//		net.ai_addr = &sa;
//		std::vector<int> fd;
//		int	newfd = accept(sfd, net.ai_addr, &net.ai_addrlen);
//		if (newfd == -1)
//		{
//			std::cout << strerror(errno) << std::endl;
//			freeaddrinfo(&net);
//			return;
//		}
//		fd.push_back(newfd);
//		std::cout << newfd << std::endl;
//		while (1)
//		{
//			read(newfd, buff, 512);
//			std::cout << buff;
//			bzero(buff, 512);
//		}
//	}
//

int	main_loop(Irc &serv)
{
	fd_set				fds[4];
	//	timeval				ttd = (timeval){2, 0};

	for (int i = 0; i < 4; i++)
		FD_ZERO(&fds[i]);
	FD_SET(serv.getSocket(), &fds[MASTER]);
	while (1)
	{
		for (int i = 1; i < 4; i++)
			fds[i] = fds[MASTER];
		std::cout << "Connecting" << std::endl;
		if (select(serv.computeFdMax(), &fds[READ], NULL, NULL, NULL) == -1)
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

	try
	{
		Irc	ircserv("ircd", "");
		ircserv.printAi();
		main_loop(ircserv);
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	return (0);
}
