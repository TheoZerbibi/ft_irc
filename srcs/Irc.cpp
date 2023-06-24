#include "ft_irc.hpp"

Irc::Irc(std::string port, std::string passwd, std::string name):
command(),
_channels(),
_clients(),
commandList(),
_net(),
_name(name),
_pass(passwd),
_sockfd(),
_replies()
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
	
	this->_removeAllChannel();
	this->_removeAllClient();
	this->_removeAllCommands();

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

void	Irc::initCommand() {
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

void	Irc::_removeAllCommands() {
	std::map<std::string, Command*>::iterator beg = this->commandList.begin();
	std::map<std::string, Command*>::iterator end = this->commandList.end();

	while (beg != end)
	{
		delete beg->second;
		beg++;
	}
}

// Getter
std::map<std::string, Command*>	Irc::getCommandList() {
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
	std::string nick = client->getNick();
	std::string user = client->getUser();
	std::string host = client->getHost();
	std::string real = client->getRealname();
	int	    fd = client->getSockfd();
	bool	    isAuth = client->isAuth();

	if (nick != "*" && !user.empty()
		&& !host.empty() && !real.empty() && isAuth) {
		Irc	&ircserv = Irc::getInstance();

		ircserv.addReply(Reply(fd, RPL_WELCOME(ircserv.getName(), nick)));
		ircserv.addReply(Reply(fd, RPL_YOURHOST(ircserv.getName(), nick)));
		ircserv.addReply(Reply(fd, RPL_INFO(ircserv.getName(), nick)));

		std::map<int, Client *>::iterator client_it = _clients.find(client->getSockfd());

		User	*user = new User(client);
		delete client_it->second;
		client_it->second = user;
	}
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

void	Irc::_removeAllClient() {
	std::map<int, Client *>::iterator beg = this->_clients.begin();
	std::map<int, Client *>::iterator end = this->_clients.end();

	while (beg != end)
	{
		delete beg->second;
		beg++;
	}
}

// Channel Management

std::map<std::string, Channel *>	&Irc::getChannelList() {
	return (this->_channels);
}

Channel	*Irc::addChannel(std::string name)
{
	if (this->channelExists(name))
		return (NULL);
	Channel	*channel = new Channel(name);

	_channels.insert(std::pair<std::string, Channel *>(name, channel));
	return (channel);
}

bool	Irc::channelExists(std::string name)
{
	std::map<std::string, Channel *>::iterator it = _channels.find(name);

	if (it != _channels.end())
		return (true);
	return (false);
}

Channel	*Irc::getChannelByName(std::string name)
{
	if (!this->channelExists(name))
		return NULL;
	return (this->_channels[name]);
}

void	Irc::removeChannel(std::string name)
{
	if (this->channelExists(name))
		return ;
	std::map<std::string, Channel *>::iterator it = _channels.begin();
	std::map<std::string, Channel *>::iterator ite = _channels.end();

	while (it != ite)
	{
		if (it->first == name)
		{
			delete it->second;
			_channels.erase(it);
			return ;
		}
		it++;
	}
}

void	Irc::addUserToChannel(User *user, Channel *chan)
{
	chan->addUser(user);
}

void Irc::_removeAllChannel() {
	std::map<std::string, Channel *>::iterator beg = this->_channels.begin();
	std::map<std::string, Channel *>::iterator end = this->_channels.end();

	while (beg != end)
	{
		delete beg->second;
		beg++;
	}
}
