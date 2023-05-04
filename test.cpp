#include <exception>
#include <iostream>
#include <errno.h>
#include <string.h>

int	main(void)
{
//	try
//	{
//		int i = 2;
//		if (i == 0)
//		{
//			throw std::exception();
//		}
//	}
//	catch (std::exception &e)
//	{
//	}

	std::cout << strerror(errno) << std::endl;
	std::cout << errno << std::endl;
	return (0);
}
