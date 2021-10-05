# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/02/07 14:21:44 by pitriche          #+#    #+#              #
#    Updated: 2021/09/17 16:16:53 by pitriche         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

RED		= \033[031m
GREEN	= \033[032m
CYAN	= \033[036m
RESET	= \033[0m

# **************************************************************************** #

NAME = ft_newton

CLASSES=	\
All		\
Display	\
Event	\
Game	\
Matrix	\
Object	\
OpenGL	\
Time	\
Utils	\

CLASSES_HEADERS= $(addsuffix .hpp, $(CLASSES))
CLASSES_SRCS= $(addsuffix .cpp, $(CLASSES))

HEADERS = $(CLASSES_HEADERS) \
General.hpp	\

SRC_FILES = $(CLASSES_SRCS)	\
main.cpp	\


# **************************************************************************** #

# if linux, it doesn't work anyway because of opengl
CC = g++

# if macos
ifeq ($(uname), Darwin)
	CC = clang++
endif


FL_OPTI = -O3 -flto
FLAGS = -Wall -Wextra -Wconversion -Wunused -std=c++11 $(FL_OPTI) \
-D 'GL_SILENCE_DEPRECATION'
# MacOS is a demon spawned low class abomination. There are no words in the
# tongues of men to describe exactly how much i hate apple for doing this


CFLAGS = -c $(FLAGS)

FRAMEWORKS = -lSDL2 -LLibrary/lib -ldl -lpthread -framework OpenGL


CINCLUDE = -I include -I Library/include/SDL2

SRC_DIR = src/
SRC := $(addprefix $(SRC_DIR), $(SRC_FILES))

INCLUDE_DIR = include/
INCLUDE := $(addprefix $(INCLUDE_DIR), $(HEADERS))

OBJ_FILES = $(patsubst %.cpp, %.o, $(SRC_FILES))
OBJ_DIR = obj/
OBJ := $(addprefix $(OBJ_DIR), $(OBJ_FILES))

all: $(NAME)
	@echo "$(CYAN)$(NAME) ready sir !$(RESET)"

$(NAME): $(OBJ_DIR) $(OBJ)
	@echo "$(GREEN)objects done sir !$(RESET)"
	@$(CC)  $(OBJ) $(FRAMEWORKS) -o $(NAME)
	@echo "$(GREEN)$(NAME) compiled sir !$(RESET)"

$(OBJ_DIR)%.o : $(SRC_DIR)%.cpp $(INCLUDE)
	@$(CC) $(CINCLUDE) $(CFLAGS) -o $@ $<
	@echo -n '.'

$(OBJ_DIR) :
	@mkdir $(OBJ_DIR)
	@echo "$(GREEN)Object directory created sir !$(RESET)"

clean:
	@rm -f $(OBJ)
	@echo "$(RED)Objects deleted sir !$(RESET)"
	@rm -rf $(OBJ_DIR)
	@#echo "$(RED)Object directory deleted sir !$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(RED)$(NAME) deleted sir !$(RESET)"

re: fclean all
