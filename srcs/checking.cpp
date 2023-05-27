#include "ft_irc.hpp"

//	bool	ft_check_input(int ac, char **av)
//	{
//		std::string		port;
//		std::string::iterator	beg;
//		std::string::iterator	end;
//	
//		if (ac != 3)
//		{
//			std::cout << "Wrong usage: format must be './ircserv <port> <password>'" << std::endl;
//			return (1);
//		}
//		port = av[1];
//		beg = port.begin(), end = port.end();
//		while (beg != end)
//		{
//			if (!std::isdigit(*beg))
//			{
//				std::cout << "Wrong port format: '" << port << "' should only contain digit" << std::endl;
//				return (1);
//			}
//			beg++;
//		}
//		return (0);
//	}
