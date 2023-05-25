NAME=ircserv

SRC_NAME=main.cpp \
	Irc.cpp \
	User.cpp \
	signal.cpp \
	client_input.cpp \
	setsocket.cpp \
	Channel.cpp

HDR_NAME=ft_irc.hpp \
	Irc.hpp \
	User.hpp

HDRS=$(addprefix $(INC), $(HDR_NAME))

INC=inc/

SRC_DIR=srcs/

OBJ_NAME=$(SRC_NAME:.cpp=.o)

OBJ_DIR=objs/

OBJS=$(addprefix $(OBJ_DIR), $(OBJ_NAME))

SRCS=$(addprefix $(SRC_DIR), $(SRC_NAME))

CXX=c++

DEBUG=0

ifeq ($(DEBUG), 1)
CXXFLAGS=-Wall -Wextra -Werror -std=c++98 -g
else
CXXFLAGS=-Wall -Wextra -Werror -std=c++98
endif

all: $(NAME)

$(NAME) : $(OBJS) $(HDR)
	$(CXX) -I$(INC) -o $(NAME) $(OBJS) $(CXXFLAGS)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp $(OBJ_DIR)
	$(CXX) -I$(INC) -o $@ -c $< $(CXXFLAGS)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

debug:
	make DEBUG=1

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
