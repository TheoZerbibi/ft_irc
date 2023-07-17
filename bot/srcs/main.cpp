#include "Bot.hpp"

int	main(int argc, char *argv[])
{

	if (argc != 4)
	{
		std::cerr << "Usage: ./bot <host> <port> <password>" << std::endl;
		return (-1);
	}

	Bot	*bot = new Bot("norminet");

	bot->connect(argv[1], std::atoi(argv[2]), argv[3]);
	bot->run();
	return (0);
}
