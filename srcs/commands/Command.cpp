#include "Command.hpp"

Command::Command(void) :
_name("Default")
{}

Command::~Command(void)
{}

void Command::execute(int fds, Client *client)
{
	(void) fds;
	(void) client;
	std::cout << this->_name << std::endl;
}

std::vector<std::string> Command::splitStr(const std::string &cmd, const char sep)
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
		end = cmd.find(sep, start);     // if not find end == npos, it's ok
		tokens.push_back(cmd.substr(start, end - start)); //it will simply substr until end of cmd
		start = cmd.find_first_not_of(sep, end);
	}
	return (tokens);
}

std::vector<std::string> Command::splitArguments(const std::string &cmd)
{
	std::vector<std::string> tokens = splitStr(cmd, ' ');
	tokens.erase(tokens.begin());
	return (tokens);
}

size_t			is_mode(char c)
{
	std::string modes = MODES;
	size_t		pos = modes.find(c);

	return ((pos == std::string::npos ? -1 : pos));
}
