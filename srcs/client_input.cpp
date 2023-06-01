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
		std::cout << ">> New connection done" << std::endl;
		return (1);
	}
	return (0);
}

int	Irc::receive_client_data(Client *user)
{
		int		fd;
		std::string	Ctype = "Client";

		fd = user->getSockfd();
		if (FD_ISSET(fd, &fds[READ]))
		{
			if (user->isRegistered())
				Ctype = "User";
			std::cout << Ctype;
			std::cout << "--- > New message from fd : " << fd <<  std::endl;
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
	std::map<int, Client*>::iterator	beg = this->getClients().begin();
	std::map<int, Client*>::iterator	end = this->getClients().end();

//	std::map<int, User>::iterator	beg = this->getUsers().begin();
//	std::map<int, User>::iterator	end = this->getUsers().end();

	while (beg != end)
	{
		if (!this->receive_client_data(beg->second))
		{
			delete beg->second;
			_clients.erase(beg++);
		}
		else
		{
			this->promote_client(beg);
			beg++;
		}
	}
	return (0);
}

// Need to change iteration to take userlist instead of an index
int	Irc::manage_incoming_connection()
{
	std::cout << "---- Reading fds ---" << std::endl;
	this->accept_client();
	this->data_reception_handler();
	return (0);
}

static std::string getCommand(std::string cmd)
{
	std::string	res = "";

	if (cmd.empty())
		return res;
	if (cmd.find(' ') != std::string::npos)
		res = cmd.substr(0, cmd.find(' '));
	else if (cmd.find('\n') != std::string::npos)
		res = cmd.substr(0, cmd.find('\n'));
	return (res);
}

int Irc::manageCommand()
{
	std::cout << "---- Manage Command ---" << std::endl;
	std::map<int, Client*>::iterator	beg = this->getClients().begin();
	std::map<int, Client*>::iterator	end = this->getClients().end();
	std::map<std::string, Command*>		commandList = this->getCommandList();

	while (beg != end)
	{
		if (beg->second->getCmds().size() > 0)
		{
			std::string cmd = beg->second->getCmds().front();
			cmd = getCommand(cmd);
			std::cout << "Command : " << cmd << std::endl;
			if (cmd.empty()) continue ;

			if (cmd == "CAP") {
				beg->second->getCmds().pop_front();
				if (beg->second->getCmds().size() > 0)
					cmd = beg->second->getCmds().front();
			}
			cmd = getCommand(cmd);
			if (cmd.empty()) {
				beg->second->getCmds().pop_front();
				continue ;
			}
			if (commandList.find(cmd) != commandList.end())
				commandList[cmd]->execute(beg->first, beg->second);
			else
				std::cout << "Command " << cmd << " found" << std::endl;
			beg->second->getCmds().pop_front();
		}
		beg++;
	}
	return (0);
}
