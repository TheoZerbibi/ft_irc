# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: thzeribi <thzeribi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/12/19 12:13:48 by thzeribi          #+#    #+#              #
#    Updated: 2023/07/19 15:25:21 by thzeribi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

################################################################################
#                                     CONFIG                                   #
################################################################################

ifndef VERBOSE
	MAKEFLAGS += --no-print-directory --silent
endif

NAME			:=	ircserv
PROJECT_NAME	:=	IRC

################################################################################
#                                 SOURCES                                      #
################################################################################
SOURCES_FOLDER		:=	./srcs/
OBJECTS_FOLDER		:=	./objs/
INCLUDES_FOLDER 	:=	./inc/
BOT_FOLDER			:=	./bot/

SOURCES				:= $(shell find ${SOURCES_FOLDER} -name "*.cpp")

################################################################################
#                                  OBJETS                                      #
################################################################################

OBJECTS		:=	$(SOURCES:.cpp=.o)
OBJECTS		:=	$(subst $(SOURCES_FOLDER),$(OBJECTS_FOLDER), $(OBJECTS))
DEPS		:=	$(OBJECTS:.o=.d)


################################################################################
#                                  FLAGS                                       #
################################################################################

CXXFLAGS		=	-MMD -Wall -Wextra -Werror -Weffc++ -std=c++98
CXX				?=	c++
DBG				:=	0

INCLUDES		:= $(addprefix -I,$(shell find ${INCLUDES_FOLDER} -type d -print))

ifneq (,$(wildcard ./.DBG.*))
	ifeq ($(findstring $(MAKECMDGOALS), re),)
		FILE := $(shell find . -name ".DBG.*" -type f)
		DBG := $(shell echo $(FILE) | rev | cut -d "." -f1 | rev )
	endif
endif

ifeq ($(DBG), 1)
	CXXFLAGS += -g3 -fsanitize=address -fsanitize=undefined -fsanitize=leak
	DEBUG := $(DBG)
else ifeq ($(DBG), 2)
	CXXFLAGS += -g3
	DEBUG := $(DBG)
else ifeq ($(DBG), 3)
	CXXFLAGS := -Werror -Weverything -Wno-padded -Wno-suggest-destructor-override -Wno-suggest-override -std=c++98 -MMD
	DEBUG := $(DBG)
else
	DEBUG := 0
endif

################################################################################
#                                 COLORS                                       #
################################################################################

NO_COLOR 		:=	\033[38;5;15m
OK_COLOR		:=	\033[38;5;2m
ERROR_COLOR		:=	\033[38;5;1m
WARN_COLOR		:=	\033[38;5;3m
SILENT_COLOR	:=	\033[38;5;245m
INFO_COLOR		:=	\033[38;5;140m
OBJ_COLOR		:=	\033[0;36m

################################################################################
#                                 RULES                                        #
################################################################################

all: header $(NAME)

$(NAME): $(OBJECTS)
	printf "\t\t$(NO_COLOR)All objects for $(INFO_COLOR)$(PROJECT_NAME) $(NO_COLOR)where successfully created.\n"
	printf "\t\t$(INFO_COLOR)$(PROJECT_NAME) $(NO_COLOR)Removed all objects$(NO_COLOR).\n"
	printf "\n\t\t$(NO_COLOR)-------- $(INFO_COLOR) Start Compilation for ${PROJECT_NAME} $(NO_COLOR)--------\n"	
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(NAME) $(OBJECTS)
	printf "%-50s \r"
	printf "\t\t$(INFO_COLOR)$(NAME)$(NO_COLOR) successfully compiled. $(OK_COLOR)✓$(NO_COLOR)\n"

force: $(OBJECTS)
	printf "\t\t\t$(NO_COLOR)All objects for $(INFO_COLOR)$(PROJECT_NAME) $(NO_COLOR)where successfully created.\n"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(NAME) $(OBJECTS)
	printf "%-50s \r"
	printf "\t\t\t$(INFO_COLOR)$(NAME)$(NO_COLOR) successfully force compiled. $(OK_COLOR)✓$(NO_COLOR)\n"

-include $(DEPS)
$(OBJECTS_FOLDER)%.o: $(SOURCES_FOLDER)%.cpp .DBG.$(DEBUG)
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
	printf "%-50s \r"
	printf "\t\t\t$(NO_COLOR)Creating $(INFO_COLOR)%-30s $(OK_COLOR)✓$(NO_COLOR)\r" "$@"

bot:
ifneq (,$(wildcard ./irc_bot))
	rm irc_bot
endif
	make -C $(BOT_FOLDER)
	ln -s $(BOT_FOLDER)irc_bot .
clean: header
	rm -f $(OBJECTS)
	make -C $(BOT_FOLDER) clean

fclean: clean
	rm -f $(NAME)
	rm -rf $(OBJECTS_FOLDER)
	rm -f .DBG.*
	make -C $(BOT_FOLDER) fclean
	printf "\t\t$(INFO_COLOR)$(PROJECT_NAME) $(NO_COLOR)Removed $(INFO_COLOR)$(NAME)$(NO_COLOR).\n"

re: fclean all

.PHONY: all re clean fclean force anclarma bot

-include ./Templates/header.mk ./Templates/debug.mk ./Templates/asan.mk \
	./Templates/anclarma.mk ./Templates/coffee.mk
