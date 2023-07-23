#include "WhoCommand.hpp"

//# define RPL_WHO(server, user, channel, name, host, ) (":" + server + " 336 " + user + " " + channel + "\r\n")
//# define RPL_ENDOFWHO(server, client) (":" + server + " 337 " + client + " :End of /INVITE list\r\n")

WhoCommand::WhoCommand(void) :
_name("WHO")
{
	std::cout << "┗⮕ WhoCommand." << std::endl;
}

WhoCommand::~WhoCommand(void)
{}

bool
	WhoCommand::cantExecute(Client *client)
{
	(void) client;
	return (true);
}

void WhoCommand::execute(int fds, Client *client)
{
	(void) fds;
	(void) client;
	std::cout << "[" << this->_name << "] : WhoCommand executed !" << std::endl;
}
