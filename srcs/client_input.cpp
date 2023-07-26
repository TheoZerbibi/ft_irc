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
			if (user->recvData() <= 0)
			{
				FD_CLR(fd, &fds[MASTER]);
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
	std::map<int, Client*>::iterator	beg = this->getClients().begin();
	std::map<int, Client*>::iterator	end = this->getClients().end();
	std::map<std::string, Command*>		commandList = this->getCommandList();
	std::map<int, Client*>::iterator	next;
	Irc									&ircserv = Irc::getInstance();

	while (beg != end)
	{
		if (beg->second && beg->second->getCmds().size() > 0)
		{
			std::string cmd = beg->second->getCmds().front();
			cmd = getCommand(cmd);
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
				if (!commandList[cmd]->cantExecute(beg->second)) {
					beg->second->getCmds().pop_front();
					continue ;
				}
				commandList[cmd]->execute(beg->first, beg->second);
			}
			else
				std::cout << "Command " << cmd << " not found" << std::endl;
			beg->second->getCmds().pop_front();
		}
		else
			beg++;
	}
	ircserv.leaveUsers();
	return (0);
}
