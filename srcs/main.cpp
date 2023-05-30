#include "ft_irc.hpp"

int	Irc::main_loop()
{
	int				fdmax;
	//	timeval				ttd = (timeval){2, 0};

	for (int i = 0; i < 4; i++)
		FD_ZERO(&(this->fds[i]));
	FD_SET(this->getSocket(), &(this->fds[MASTER]));
	while (1)
	{
		for (int i = 1; i < 4; i++)
			(this->fds[i] = this->fds[MASTER]);
		fdmax = this->computeFdMax();
		std::cout << "Connecting" << std::endl;
		if (select(fdmax, &(this->fds[READ]), NULL, NULL, NULL) == -1)
		{
			std::cerr << "Select error : ";
			throw  SyscallError();
		}
		this->manage_incoming_connection();
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
		ircserv.main_loop();
	}
	catch (std::exception &e)
	{
		if (strcmp(e.what(), "Success"))
			std::cerr << e.what() << std::endl;
	}
	return (0);
}
