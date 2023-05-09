#include <exception>
#include <iostream>
#include <errno.h>
#include <string.h>


class	Test{
	public:
		Test();
		~Test();
		void	print() { std::cout << "printing things" << std::endl; }
};

Test::Test()
{
	std::cout << "Hello im the start of test" << std::endl;
}

Test::~Test()
{
	std::cout << "Goodbye" << std::endl;
}

int	main(void)
{

	try
	{
		Test	test;
		throw	std::exception();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	test.print();

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
//	std::cout << strerror(errno) << std::endl;
//	std::cout << errno << std::endl;
	return (0);
}
