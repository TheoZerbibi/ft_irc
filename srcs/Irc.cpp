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
	Client	* client = new Client(sfd);

	_clients.insert(std::make_pair(sfd, client));
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

//	Irc	&Irc::operator=(const Irc &rhs)
//	{
//		int	status;
//	
//		status = getaddrinfo(NULL, NULL, rhs.getAi(), &this->_net);
//		freeaddrinfo(this->_net);
//		this->_sockfd = rhs.getSocket();
//		this->_users = rhs.getUsers();
//		return (*this);
//	}
