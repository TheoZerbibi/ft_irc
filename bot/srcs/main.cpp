#include "Bot.hpp"
#include "openai.hpp"

int	main(int argc, char *argv[])
{

	if (argc != 4)
	{
		std::cerr << "Usage: ./bot <host> <port> <password>" << std::endl;
		return (-1);
	}

	Bot	bot = Bot::getInstance("norminet");

	bot.connect(argv[1], std::atoi(argv[2]), argv[3]);
	openai::start();
	bot.run();
	return (0);
}
