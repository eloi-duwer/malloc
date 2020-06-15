# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/05/22 16:26:19 by eduwer            #+#    #+#              #
#    Updated: 2020/02/14 12:22:02 by eduwer           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc_$(HOSTTYPE).so
SOFTLINK = libft_malloc.so
TEST_NAME = test_malloc

SRC_LIB = free.c \
	malloc.c \
	mmap_zones.c \
	realloc.c \
	shift.c \

SRC_TEST = tests/test.c \

CC = gcc

CFLAGS = -I./include -I./libft/include -Wall -Wextra -fpic

SRCF = ./src/

OBJF = ./obj/lib/
OBJF_TESTS = ./obj/tests/

OBJS = $(addprefix $(OBJF), $(SRC_LIB:.c=.o))
OBJS_TESTS = $(addprefix $(OBJF_TESTS), $(SRC_TEST:.c=.o))

LFT = libft/libft.a

all : $(NAME)

$(NAME) : $(LFT) $(OBJS)
	$(CC) -shared -o $(NAME) $(OBJS)
	@rm -rf $(SOFTLINK)
	ln -s $(NAME) $(SOFTLINK)

$(LFT):
	make -C ./libft

$(OBJF)%.o : $(SRCF)%.c
	@mkdir -p $(@D)
	$(CC) -o $@ $(CFLAGS) -c $(addprefix $(SRCF), $*.c)

clean :
	rm -rf	$(OBJS)
	rm -rf	$(OBJS_TESTS)

fclean : clean
	rm -rf $(NAME)
	rm -rf $(SOFTLINK)
	rm -rf $(TEST_NAME)

re : fclean all

$(OBJF_TESTS)%.o : $(SRCF)%.c
	@mkdir -p $(@D)
	$(CC) -o $@ -I./libft/include -Wall -Wextra -c $(addprefix $(SRCF), $*.c)


test: $(NAME) $(OBJS_TESTS)
	$(CC) -o $(TEST_NAME) -Wall -Wextra $(OBJS_TESTS) -L. -lft_malloc -L./libft/ -lft -Wl,-rpath=.
