#include <iostream>
#include <string>
#include <map>
#include <utility>
#include "Command.hpp"
#include "TestCommand.hpp"
#include "PingCommand.hpp"

int main(int argc, char **argv)
{

	if (argc < 2)
	{
		std::cerr << "Usage: " << argv[0] << " <command>" << std::endl;
		return (1);
	}

	std::map<std::string, Command*> commandList;
	commandList.insert(std::make_pair("test", new TestCommand()));
	commandList.insert(std::make_pair("ping", new PingCommand()));
	if (commandList.find(argv[1]) == commandList.end())
		std::cerr << "Command " << argv[1] << " not found" << std::endl;
	else
		commandList[argv[1]]->execute();
	return (0);
}
