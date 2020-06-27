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

SRC_LIB = free.c \
	malloc.c \
	mmap_zones.c \
	realloc.c \
	shift.c \
	show_alloc_mem.c \

CC = gcc

CFLAGS = -I./include -I./libft/include -Wall -Wextra -fpic -g

SRCF = ./src/

OBJF = ./obj/lib/

OBJS = $(addprefix $(OBJF), $(SRC_LIB:.c=.o))

LFT = libft/libft.a

all : $(NAME)

$(NAME) : $(LFT) $(OBJS)
	$(CC) -shared -o $(NAME) $(OBJS) $(LFT) -g
	@rm -rf $(SOFTLINK)
	ln -s $(NAME) $(SOFTLINK)

$(LFT):
	make -C ./libft

$(OBJF)%.o : $(SRCF)%.c
	@mkdir -p $(@D)
	$(CC) -o $@ $(CFLAGS) -c $(addprefix $(SRCF), $*.c)

clean :
	rm -rf	$(OBJS)

fclean : clean
	rm -rf $(NAME)
	rm -rf $(SOFTLINK)

re : fclean all
