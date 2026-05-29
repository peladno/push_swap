# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/05/27 18:30:26 by skusakab          #+#    #+#              #
#    Updated: 2026/05/29 20:18:40 by skusakab         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= push_swap
SRCS		=	src/main.c \
				src/stack/stack_init.c \
				src/stack/stack_free.c \
				src/stack/stack_push_bottom.c \
				src/parser/coordinate_compression.c
OBJS		= $(SRCS:%.c=%.o)
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -I include -I $(LIBFT_DIR)
HEADER		= include/push_swap.h
LIBFT_DIR	= ./libft
LIBFT		= $(LIBFT_DIR)/libft.a

all:	$(NAME)

$(NAME):	$(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT)

%.o:	%.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	rm -f $(OBJS)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

re:	fclean all

.PHONY: all clean fclean re
