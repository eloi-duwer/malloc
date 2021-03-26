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
	put_size_t_nbr.c \
	realloc_utils.c \
	block_control.c \
	print_mem.c

CC = gcc

CFLAGS = -I./include -I./libft/include -Wall -Wextra -fpic

SRCF = ./src/

OBJF = ./obj/

OBJS = $(addprefix $(OBJF), $(SRC_LIB:.c=.o))

LFT = libft/libft.a

all : $(NAME)

$(NAME) : $(LFT) $(OBJS)
	ld -shared -o $(NAME) $(OBJS) $(LFT) --retain-symbols-file symbols.txt
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
