#include "ft_irc.hpp"

Irc::Irc(std::string port, std::string passwd, std::string name):
command(),
_channels(),
_clients(),
_clientsWantLeave(),
commandList(),
_net(),
_name(name),
_pass(passwd),
_oper_pass("sesame"),
_sockfd(),
_replies()
{
	this->setupAddrInfo(port);
	this->setupFds();
	this->initCommand();
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
	this->commandList.insert(std::pair<std::string, Command*>("CAP", new CapCommand()));
	this->commandList.insert(std::pair<std::string, Command*>("INVITE", new InviteCommand()));
	this->commandList.insert(std::pair<std::string, Command*>("JOIN", new JoinCommand()));
	this->commandList.insert(std::pair<std::string, Command*>("KICK", new KickCommand()));
	this->commandList.insert(std::pair<std::string, Command*>("MODE", new ModeCommand()));
	this->commandList.insert(std::pair<std::string, Command*>("NICK", new NickCommand()));
	this->commandList.insert(std::pair<std::string, Command*>("PART", new PartCommand()));
	this->commandList.insert(std::pair<std::string, Command*>("PASS", new PassCommand()));
	this->commandList.insert(std::pair<std::string, Command*>("PING", new PingCommand()));
	this->commandList.insert(std::pair<std::string, Command*>("PRIVMSG", new PrivMsgCommand()));
	this->commandList.insert(std::pair<std::string, Command*>("QUIT", new QuitCommand()));
	this->commandList.insert(std::pair<std::string, Command*>("TOPIC", new TopicCommand()));
	this->commandList.insert(std::pair<std::string, Command*>("USER", new UserCommand()));
	this->commandList.insert(std::pair<std::string, Command*>("WHO", new WhoCommand()));
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

User		*Irc::getUserByNick(std::string const nick) const
{
	std::map<int, Client *>::const_iterator	beg = _clients.begin();
	std::map<int, Client *>::const_iterator	end = _clients.end();
	
	while (beg != end)
	{
		if (beg->second->isRegistered())
		{
			if (beg->second->getNickname() == nick)
				return (dynamic_cast<User *>(beg->second));
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

void	Irc::removeClient(int const &sfd, std::string const &msg)
{
	std::map<int, Client *>::iterator it = _clients.find(sfd);
	std::map<int, Client *>::iterator end = _clients.end();

	if (it != end)
	{
		if (it->second->isRegistered()) {
			User	*user = dynamic_cast<User *>(it->second);
			user->quitAllChannel(msg);
		}
		this->addReply(Reply(sfd, RPL_QUIT(user_id(this->getName(), it->second->getNickname(), it->second->getUsername()), msg)));
		delete it->second;
		_clients.erase(it);
	}
}

void	Irc::promoteClient(Client *client)
{
	std::string 	nickname = client->getNickname();
	std::string 	username = client->getUsername();
	std::string 	host = client->getHost();
	std::string 	real = client->getRealname();
	int				fd = client->getSockfd();
	bool			isAuth = client->isAuth();
	std::ifstream	motd;
	char			motdPath[20] = "ressources/MOTD.txt";

	if (nickname != "*" && !username.empty()
			&& !host.empty() && !real.empty() && isAuth) {

		this->addReply(Reply(fd, RPL_WELCOME(this->getName(), nickname, user_ids(host, nickname, username))));
		this->addReply(Reply(fd, RPL_YOURHOST(this->getName(), nickname)));
		this->addReply(Reply(fd, RPL_INFO(this->getName(), nickname)));

		motd.open(motdPath);
		if (!motd)
			this->addReply(Reply(fd, ERR_NOMOTD(this->getName(), nickname)));
		else {
			std::string line;
			std::string buf;

			buf = RPL_STARTOFMOTD(this->getName(), nickname, "42 FT_IRC");
			while (getline(motd, line))
				buf += RPL_MOTD(this->getName(), nickname, line);
			buf += RPL_ENDOFMOTD(this->getName(), nickname);
			this->addReply(Reply(fd, buf));
			motd.close();
		}

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

void	Irc::addReply(Reply reply)
{
	this->_replies.push_back(reply);
}

Reply	Irc::mergeReply()
{
	std::vector<Reply>::iterator	beg = _replies.begin();
	Reply	rpl = *beg;
	beg = _replies.erase(beg);
	std::vector<Reply>::iterator	end = _replies.end();

	while (beg != end)
	{
		if (rpl.getClientFd() != beg->getClientFd())
			beg++;
		else
		{
			rpl += *beg;
			beg = _replies.erase(beg);
			end = _replies.end();
		}
	}
	return (rpl);
}

void	Irc::mergeReplies()
{
	std::vector<Reply>	newRpls;
	Reply			rpl;

	while (!_replies.empty())
	{
		rpl = mergeReply();
		newRpls.push_back(rpl);
	}
	_replies = newRpls;
}

// erase return next element after the erased one
int	Irc::sendReplies(void)
{
	std::vector<Reply>::iterator	beg = _replies.begin();

	std::vector<Reply>		cpy = _replies;
	std::vector<Reply>::iterator	cpy_beg = cpy.begin();
	std::vector<Reply>::iterator	cpy_end = cpy.end();

	int				i  = 0;

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
	Client			*client;

	std::cout << "|--> QUITTING process" << std::endl;
	while (beg != end)
	{
		client = beg->second;
		if (client->isRegistered()) {
			User	*user = dynamic_cast<User *>(client);
			user->clearChannel();
		}
		_clients.erase(beg++);
		delete client;
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

Channel	*Irc::getChannel(std::string name)
{
	if (!this->channelExists(name))
		return NULL;
	return (this->_channels[name]);
}

void	Irc::removeChannel(Channel *channel)
{

	_channels.erase(channel->getName());
	delete channel;
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
		_channels.erase(beg++);
	}
}

bool
	Irc::_isClientWantLeave(Client *client)
{
	if (this->_clientsWantLeave.find(client->getSockfd()) != this->_clientsWantLeave.end())
		return (true);
	return (false);
}

void
	Irc::addLeaveUser(Client *client, std::string const &reason)
{
	if (!this->_isClientWantLeave(client))
		this->_clientsWantLeave.insert(std::make_pair(client->getSockfd(), reason));
}

void
	Irc::leaveUsers(void)
{
	std::map<int, std::string>::iterator beg = this->_clientsWantLeave.begin();
	std::map<int, std::string>::iterator end = this->_clientsWantLeave.end();

	while (beg != end)
	{
		this->removeClient(beg->first, beg->second);
		this->_clientsWantLeave.erase(beg++);
	}
}
