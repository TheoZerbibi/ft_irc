#include "ft_irc.hpp"

/*
**
** TODO:
**   - Main args
**   - WHOCommand?
** BUGFIX:
**   - Message when player join a channel
**   - Mode
**   - Fix /invite dont send message in chan + segfault
**   - Rework QuitCommand, add vector for kill client after client_input
**
*/

int	Irc::setup_fds()
{
	for (int i = 1; i < 4; i++)
		(this->fds[i] = this->fds[MASTER]);
	return (this->computeFdMax());
}

int	Irc::main_loop()
{
	int		ret;
	int		fdMax;
	timeval	ttd;

	ttd = (timeval){2, 0};
	while (1)
	{
		fdMax = this->computeFdMax();

		this->mergeReplies();
		this->fds[READ] = this->fds[MASTER];
		this->fds[SEND] = this->fds[MASTER];
		ret = select(fdMax, &(this->fds[READ]), &(this->fds[SEND]), NULL, &ttd);
		if (ret == -1)
		{
			std::cerr << "Select error : ";
			throw  SyscallError();
		}
		else if (ret)
		{
			this->sendReplies();
			this->manage_incoming_connection();
		}
		this->manageCommand();
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
