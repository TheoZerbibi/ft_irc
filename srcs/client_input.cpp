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
//			if (user->isRegistered())
//				Ctype = "User";
//			std::cout << Ctype;
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

	while (beg != end)
	{
		if (!this->receive_client_data(beg->second))
		{
			delete beg->second;
			_clients.erase(beg++);
		}
		else
			beg++;
	}
	return (0);
}

int	Irc::manage_incoming_connection()
{
	this->accept_client();
	this->data_reception_handler();
	return (0);
}

static std::string getCommand(std::string cmd)
{
	std::string	res = cmd;

	if (cmd.empty())
		return res;
	if (cmd.find(' ') != std::string::npos)
		res = cmd.substr(0, cmd.find(' '));
	return (res);
}

int Irc::manageCommand()
{
	// std::cout << "---- Manage Command ---" << std::endl;
	std::map<int, Client*>::iterator	beg = this->getClients().begin();
	std::map<int, Client*>::iterator	end = this->getClients().end();
	std::map<std::string, Command*>		commandList = this->getCommandList();

	while (beg != end)
	{
		if (beg->second && beg->second->getCmds().size() > 0)
		{
			std::string cmd = beg->second->getCmds().front();
			cmd = getCommand(cmd);
			std::cout << "Command : " << cmd << std::endl;
			if (cmd.empty()) {
				beg->second->getCmds().pop_front();
				continue ;
			}
			cmd = getCommand(cmd);
			if (cmd.empty()) {
				beg->second->getCmds().pop_front();
				continue ;
			}
			if (commandList.find(cmd) != commandList.end()) {
				commandList[cmd]->execute(beg->first, beg->second);
				if (cmd == "QUIT") {
					std::cout << "QUITCOMMAND" << std::endl;
					beg++;
					continue;
				}
			}
			else
				std::cout << "Command " << cmd << " not found" << std::endl;
			beg->second->getCmds().pop_front();
		}
		beg++;
	}
	return (0);
}
