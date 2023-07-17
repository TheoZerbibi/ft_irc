#include "Bot.hpp"

Bot::Bot(std::string name) :
_sa(),
_socket(),
_name(name),
_host(""),
_realname("Norminet Babache"),
_user("Norminet"),
_pass(""),
_port(0)
{
	std::cout << "┏⮕ Creating Bot " << this->_name << std::endl;
	this->setupSignal();
}

Bot::~Bot(void)
{
	std::cout << "┗⮕ Destroying Bot " << this->_name << std::endl;
}

void
	Bot::setName(std::string name)
{
	this->_name = name;
}

void
	Bot::setHost(std::string host)
{
	this->_host = host;
}

void
	Bot::setRealname(std::string realname)
{
	this->_realname = realname;
}

void
	Bot::setUser(std::string user)
{
	this->_user = user;
}

void
	Bot::setPass(std::string pass)
{
	this->_pass = pass;
}

void
	Bot::setPort(int port)
{
	this->_port = port;
}

std::string	&
	Bot::getName(void) const
{
	return ((std::string &)this->_name);
}

std::string	&
	Bot::getHost(void) const
{
	return ((std::string &)this->_host);
}

std::string	&
	Bot::getRealname(void) const
{
	return ((std::string &)this->_realname);
}

std::string	&
	Bot::getUser(void) const
{
	return ((std::string &)this->_user);
}

std::string &
	Bot::getPass(void) const
{
	return ((std::string &)this->_pass);
}

int	&
	Bot::getPort(void) const
{
	return ((int &)this->_port);
}

void
	quitBot(int signal)
{
	(void)signal;

	exit(0);
}

void
	Bot::setupSignal(void)
{
	std::cout << "┣⮕ Bot::setupSignal()" << std::endl;

	// this->_sa.sa_handler = &Bot::quit;
	this->_sa.sa_handler = &quitBot;
	this->_sa.sa_flags = 0;
	sigemptyset(&this->_sa.sa_mask);
	sigaction(SIGINT, &this->_sa, 0);
}

void
	Bot::connect(std::string host, int port, std::string password)
{
	this->_host = host;
	this->_port = port;
	this->_pass = password;

	this->_socket.link(this->_host.c_str(), this->_port);
	std::cout << "┣⮕ Bot::connect(" << this->_host << ", " << this->_port <<  ", " << this->_pass << ")" << std::endl;

	this->_socket << "USER " << this->_user << " 0 * :" << this->_realname.c_str() << "\r\n";
	this->_socket << "NICK " << this->_name << "\r\n";
	this->_socket << "PASS " << this->_pass << "\r\n";
	std::cout << "┣⮕ Bot::auth(" << this->_user << ", " << this->_name << ", " << this->_pass << ")" << std::endl;
}

void
	Bot::run(void)
{
	char	buf[513];

	while (this->_socket.in(buf)) {
		std::string msg(buf);
		if (msg.find("001") != std::string::npos)
		{
			this->_socket << "MODE " << this->_name << " +B\r\n";
			this->_socket << "JOIN #test\r\n";
		} else if (msg.find("PRIVMSG") != std::string::npos && msg.find("norminet!Norminet@") == std::string::npos)
			this->_socket << "PRIVMSG #test :Hello World!\r\n";
		std::cout << "Buff: " << msg;
	}
}

void
	Bot::quit(int signal)
{
	(void)signal;

	this->_socket.unlink();
	delete this;
	exit(0);
}
