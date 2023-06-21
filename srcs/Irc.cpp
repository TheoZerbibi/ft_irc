#include "ft_irc.hpp"

Irc::Irc()
{}

Irc::Irc(std::string port, std::string passwd, std::string name): _name(name), _pass(passwd)
{
	this->initCommand();
	this->setupAddrInfo(port);
	this->setupFds();
	std::cout << "PASS : " << this->_pass << std::endl;
}

Irc::~Irc()
{
	close(this->_sockfd);
	freeaddrinfo(this->_net);
	
	std::map<int, Client *>::iterator beg = _clients.begin();
	std::map<int, Client *>::iterator end = _clients.end();

	while (beg != end)
	{
		delete beg->second;
		beg++;
	}

	std::map<std::string, Command*>::iterator begC = commandList.begin();
	std::map<std::string, Command*>::iterator endC = commandList.end();
	while (begC != endC)
	{
		delete begC->second;
		begC++;
	}
	std::cout << "Exiting" << std::endl;

}

void	Irc::setupAddrInfo(std::string port)
{
	struct	addrinfo	hint;
	int			status;

	std::memset(&hint, 0, sizeof(hint));
	hint.ai_family = AF_UNSPEC;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_flags = AI_PASSIVE;
	status = getaddrinfo(NULL, port.c_str(), &hint, &this->_net);
	if (status)
	{
		std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
		throw SyscallError();
	}
	if ((setup_socket()) == -1)
	{
		freeaddrinfo(this->_net);
		std::cerr << "Socket creation failed: ";
		throw SyscallError();
	}
}

void	Irc::setupFds()
{
	for (int i = 0; i < 4; i++)
		FD_ZERO(&(this->fds[i]));
	FD_SET(this->getSocket(), &(this->fds[MASTER]));
}

void Irc::initCommand() {
	std::cout << "┏ Command Register" << std::endl;
	this->commandList.insert(std::pair<std::string, Command*>("AWAY", new AwayCommand()));
	this->commandList.insert(std::pair<std::string, Command*>("INVITE", new InviteCommand()));
	this->commandList.insert(std::pair<std::string, Command*>("JOIN", new JoinCommand()));
	this->commandList.insert(std::pair<std::string, Command*>("KICK", new KickCommand()));
	this->commandList.insert(std::pair<std::string, Command*>("MODE", new ModeCommand()));
	this->commandList.insert(std::pair<std::string, Command*>("NICK", new NickCommand()));
	this->commandList.insert(std::pair<std::string, Command*>("PART", new PartCommand()));
	this->commandList.insert(std::pair<std::string, Command*>("PASS", new PassCommand()));
	this->commandList.insert(std::pair<std::string, Command*>("PRIVMSG", new PrivMsgCommand()));
	this->commandList.insert(std::pair<std::string, Command*>("TOPIC", new TopicCommand()));
	this->commandList.insert(std::pair<std::string, Command*>("USER", new UserCommand()));
}

// Getter
std::map<std::string, Command*> Irc::getCommandList() {
	return this->commandList;
}

const int	&Irc::getSocket() const {
	return ((this->_sockfd));
}

const std::string &Irc::getName() const {
	return (this->_name);
}

const std::string &Irc::getPass() const {
	return (this->_pass);
}

const struct addrinfo	*Irc::getAi() const
{
	return (this->_net);
}

std::map<int, Client*>	&Irc::getClients()
{
	return ((this->_clients));
}

const Client		*Irc::getUserByNick(std::string const nick) const
{
	std::map<int, Client *>::const_iterator	beg = _clients.begin();
	std::map<int, Client *>::const_iterator	end = _clients.end();
	
	while (beg != end)
	{
		if (beg->second->isRegistered())
		{
			if (beg->second->getNick() == nick)
				return (beg->second);
		}
		++beg;
	}
	return (NULL);
}



//Setter
void	Irc::addClient(int const &sfd)
{
	Client	*client = new Client(sfd);

	_clients.insert(std::make_pair(sfd, client));
}

void	Irc::promoteClient(Client *client)
{
	std::map<int, Client *>::iterator client_it = _clients.find(client->getSockfd());

	User	*user = new User(client);
	delete client_it->second;
	client_it->second = user;
}

void	Irc::printAi() const
{
	std::cout << "Address family: ";
	if (_net->ai_family == AF_INET)
		std::cout << "Ipv4" << std::endl;
	else if (_net->ai_family == AF_INET6)
		std::cout << "Ipv6" << std::endl;
	else
		std::cout << "Unspecified" << std::endl;
	std::cout << "ip : " << inet_ntoa(((struct sockaddr_in *)_net->ai_addr)->sin_addr) << std::endl;
	std::cout << "Port : " << ntohs(((struct sockaddr_in *)_net->ai_addr)->sin_port) << std::endl;
}

// Client Management

int	Irc::computeFdMax(void) const
{
	int	fdmax = this->getSocket();
	int	curr_fd;
	std::map<int, Client*>::const_iterator it = this->_clients.begin();
	std::map<int, Client*>::const_iterator ite = this->_clients.end();

	while (it != ite)
	{
		curr_fd = (*it).first;
		if (fdmax < curr_fd)
			fdmax = curr_fd;
		it++;
	}
	return (fdmax + 1);
}

void Irc::addReply(Reply reply)
{
	this->_replies.push_back(reply);
}

int	Irc::sendReplies(void)
{
	std::vector<Reply>::iterator	beg = _replies.begin();

	std::vector<Reply>		cpy = _replies;
	std::vector<Reply>::iterator	cpy_beg = cpy.begin();
	std::vector<Reply>::iterator	cpy_end = cpy.end();

	int				i  = 0;

	// std::cout << "<<-- Sending Replies" << std::endl;
	while (cpy_beg + i != cpy_end)
	{
		if (FD_ISSET((cpy_beg + i)->getClientFd(), &(this->fds[SEND])))
		{
			if (!(cpy_beg + i)->send())
			{
				_replies.erase(beg + i);
			}
		}
		i++;
	}
	return (0);
}
