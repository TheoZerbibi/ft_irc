NAME=ircserv
SRC_NAME=main.cpp \
	ft_irc.cpp \
    setsocket.cpp

HDR_NAME=ft_irc.hpp\
	User.hpp

HDRS=$(addprefix $(INC), $(HDR_NAME))

INC=inc/

SRC_DIR=srcs/

OBJ_NAME=$(SRC_NAME:.cpp=.o)

OBJ_DIR=objs/

OBJS=$(addprefix $(OBJ_DIR), $(OBJ_NAME))

SRCS=$(addprefix $(SRC_DIR), $(SRC_NAME))

CXX=c++
CXXFLAGS=-Wall -Wextra -Werror -std=c++98

all: $(NAME)

$(NAME) : $(OBJS) $(HDR)
	$(CXX) -o $(NAME) $(OBJS) $(CXXFLAGS)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp $(OBJ_DIR)
	$(CXX) -o $@ -c $< $(CXXFLAGS) -I$(INC)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
