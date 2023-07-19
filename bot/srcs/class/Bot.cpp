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
	this->quit();
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
	if (this->_socket.getFd() == 0)
	{
		std::cerr << "┣⮕ Bot::connect(" << this->_host << ", " << this->_port <<  ", " << this->_pass << ") failed" << std::endl;
		exit(1);
	}
	std::cout << "┣⮕ Bot::connect(" << this->_host << ", " << this->_port <<  ", " << this->_pass << ")" << std::endl;

	this->_socket << "USER " << this->_user << " 0 * :" << this->_realname.c_str() << "\r\n";
	this->_socket << "NICK " << this->_name << "\r\n";
	this->_socket << "PASS " << this->_pass << "\r\n";
	std::cout << "┣⮕ Bot::auth(" << this->_user << ", " << this->_name << ", " << this->_pass << ")" << std::endl;
}

std::string
	Bot::useOpenAi(std::vector<std::string> args)
{
	if (args.at(2).empty())
		return ("");

	std::cout << "Question : " << args.at(2) << std::endl;
	auto request = R"({
			"model": "text-davinci-003",
			"prompt": "",
			"max_tokens": 100,
			"temperature": 0
		})"_json;
	request["prompt"] = args.at(2);
	auto completion = openai::completion().create(request);
	if (completion.contains("choices") && completion["choices"].is_array() &&
		completion["choices"].size() > 0) {
		std::string text;
		for (const auto& choice : completion["choices"]) {
			text += choice["text"];
		}
		text.erase( std::remove(text.begin(), text.end(), '\r'), text.end() );
		text.erase( std::remove(text.begin(), text.end(), '\n'), text.end() );
		std::cout << "Response is:\n" << completion.dump(2) << '\n';
		std::cout << "Text from completion: " << text << std::endl;
		return (text);
	} else {
		return ("");
	}
}

void
	Bot::parseMessage(std::string msg)
{
	std::vector<std::string>	args = this->splitArguments(msg);

	if (args.at(0) == "PING")
		this->_socket << "PONG " << args.at(1) << "\r\n";
	else if (args.at(0) == "PRIVMSG") {
		if (args.at(1).find("#") == std::string::npos) {
			unsigned	last = msg.find_first_of("!");
			std::string	nick = msg.substr(1, last-1);
			std::string response = this->useOpenAi(args);
			if (response.empty())
				return ;
			this->_socket << "PRIVMSG " << nick << " : " << response<< "\r\n";
		} else if (args.at(2).find("@norminet") != std::string::npos) {
			std::string response = this->useOpenAi(args);
			if (response.empty())
				return ;
			this->_socket << "PRIVMSG " << args.at(1) << " : " << response << "\r\n";
		}
	}
	else if (args.at(0) == "INVITE") {
		this->_socket << "JOIN " << args.at(2) << "\r\n";
		std::cout << "Joinning channel : " << args.at(2) << std::endl;
	}
}

void
	Bot::run(void)
{
	char	buf[513];

	while (this->_socket.in(buf)) {
		std::string msg(buf);
		if (msg.find("001") != std::string::npos)
			this->_socket << "MODE " << this->_name << " +B\r\n";
		else if (msg.find("norminet!Norminet@") == std::string::npos)
			this->parseMessage(msg);
	}
}

void
	Bot::quit()
{
	this->_socket.unlink();
}

std::vector<std::string> Bot::splitStr(const std::string &cmd, const char sep)
{
	std::vector<std::string> tokens;
	std::string				tmp;
	std::size_t start = 0, end = 0;

	while (start != std::string::npos)
	{
		if (start != 0 && cmd.at(start) == ':')
		{
			tokens.push_back(cmd.substr(start + 1));
			return (tokens);
		}
		end = cmd.find(sep, start);
		tokens.push_back(cmd.substr(start, end - start));
		start = cmd.find_first_not_of(sep, end);
	}
	return (tokens);
}

std::vector<std::string> Bot::splitArguments(const std::string &cmd)
{
	std::vector<std::string> tokens = splitStr(cmd, ' ');
	tokens.erase(tokens.begin());
	return (tokens);
}
