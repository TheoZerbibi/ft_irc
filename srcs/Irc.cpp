#include "ft_irc.hpp"

Irc::Irc()
{}

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

Irc::Irc(std::string port, std::string passwd): _pass(passwd)
{
	struct	addrinfo	hint;
	int			status;

	this->initCommand();
	(void)port;
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
	if (this->_pass.empty())
		this->_pass = "123";
	std::cout << "PASS : " << this->_pass << std::endl;
}

void Irc::initCommand() {
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

//Setter
void	Irc::addClient(int const &sfd)
{
	Client	*client = new Client(sfd);

	_clients.insert(std::make_pair(sfd, client));
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

void	Irc::promote_client(std::map<int, Client *>::iterator &client_it)
{
	Client *client = client_it->second;

	if (client->isRegistered())
	{
		std::cout << "Client " << client->getUser() << "is already registered" << std::endl;
		return ;
	}
	User	*user = new User(client_it->second);
	delete client_it->second;
	client_it->second = user;
}

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

int	Irc::sendReplies(void)
{
	std::cout << "<<-- Sending Replies" << std::endl;
	return (0);
}
