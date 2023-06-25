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

std::vector<std::string> splitStr(const std::string &cmd, const char sep)
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

std::vector<std::string> splitArguments(const std::string &cmd)
{
	std::vector<std::string> tokens = splitStr(cmd, ' ');
	tokens.erase(tokens.begin());
	return (tokens);
}

// Probleme !
// tokenize apres une occurence de :
// Ignore totalement le dernier token lorsqu'il n'est pas suivie par au moins un espace
// string::substr prend une len en tant que second argument et non pas une position

//	std::vector<std::string> splitArguments(const std::string &cmd)
//	{
//		std::vector<std::string> tokens;
//		std::size_t start = 0, end = 0;
//	
//		while ((end = cmd.find(' ', start)) != std::string::npos) {
//			if (end != start) {
//				std::string tmp = cmd.substr(start, end - start);
//				if (tmp[0] == ':') {
//					tmp = cmd.substr(start, end);
//					tmp = tmp.substr(1);
//				}
//				std::cout << tmp << std::endl;
//				tokens.push_back(tmp);
//			}
//			start = end + 1;
//		}
//		return tokens;
//	}
