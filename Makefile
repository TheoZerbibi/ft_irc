NAME=ircserv
SRC=main.cpp
CXX=c++
CXXFLAGS=-Wall -Wextra -Werror -std=c++98

OBJ=$(SRC:.cpp=.o)

all: $(NAME)

$(NAME) : $(OBJ)
	$(CXX) -o $(NAME) $(OBJ) $(CXXFLAGS)

%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
