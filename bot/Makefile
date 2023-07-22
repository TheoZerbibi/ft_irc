# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: thzeribi <thzeribi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/19 13:19:33 by thzeribi          #+#    #+#              #
#    Updated: 2023/07/19 13:48:47 by thzeribi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifndef VERBOSE
	MAKEFLAGS += --no-print-directory --silent
endif

NAME			:=	irc_bot
PROJECT_NAME	:=	IRC_BOT

################################################################################
#                                 SOURCES                                      #
################################################################################

SOURCES_FOLDER	:= ./srcs/
OBJECTS_FOLDER	:= ./objs/
INCLUDES_FOLDER	:= ./privates/

SOURCES			:= $(shell find ${SOURCES_FOLDER} -name "*.cpp")

################################################################################
#                                  OBJETS                                      #
################################################################################

OBJECTS	:= $(SOURCES:.cpp=.o)
OBJECTS	:= $(subst $(SOURCES_FOLDER),$(OBJECTS_FOLDER),$(OBJECTS))
DEPS	:= $(OBJECTS:.o=.d)

################################################################################
#                                  FLAGS                                       #
################################################################################

CXXFLAGS	:= -MMD -Wall -Wextra -Werror
CXX			?= g++
DBG			:=	0

LDFLAGS		:= -L/$(HOME)/.config/curl/lib -lcurl
INCLUDES	:= $(addprefix -I,$(shell find ${INCLUDES_FOLDER} -type d -print))
INCLUDES	+= -I/$(HOME)/.config/curl/include

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
	CXXFLAGS := -Werror -Weverything -Wno-padded -Wno-suggest-destructor-override -Wno-suggest-override -MMD
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
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS)
	printf "%-50s \r"
	printf "\t\t$(INFO_COLOR)$(NAME)$(NO_COLOR) successfully compiled. $(OK_COLOR)✓$(NO_COLOR)\n"

-include $(DEPS)
$(OBJECTS_FOLDER)%.o: $(SOURCES_FOLDER)%.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
	printf "%-50s \r"
	printf "\t\t\t$(NO_COLOR)Creating $(INFO_COLOR)%-30s $(OK_COLOR)✓$(NO_COLOR)\r" "$@"

clean: header
	rm -f $(OBJECTS)

fclean: clean
	rm -f $(NAME)
	rm -rf $(OBJECTS_FOLDER)
	rm -f .DBG.*
	printf "\t\t$(INFO_COLOR)$(PROJECT_NAME) $(NO_COLOR)Removed $(INFO_COLOR)$(NAME)$(NO_COLOR).\n"

re: fclean all

.PHONY: all re clean fclean

-include ../Templates/header.mk ../Templates/debug.mk ../Templates/asan.mk \
	../Templates/anclarma.mk ../Templates/coffee.mk
