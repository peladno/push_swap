# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: skusakab <skusakab@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/05/27 18:30:26 by skusakab          #+#    #+#              #
#    Updated: 2026/06/20 13:29:35 by skusakab         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= push_swap
SRCS		=	src/main.c \
				src/stack/create_node.c \
				src/stack/stack_init.c \
				src/stack/stack_free.c \
				src/stack/stack_push_bottom.c \
				src/parser/coordinate_compression.c \
				src/parser/validate_token.c \
				src/parser/free_split.c \
				src/parser/has_duplicates.c \
				src/parser/parse_args.c \
				src/operations/swap.c \
				src/operations/push.c \
				src/operations/rotate.c \
				src/operations/reverse_rotate.c \
				src/algorithms/complex/radix_sort.c \
				src/algorithms/medium/chunk_sort.c \
				src/algorithms/medium/chunk_sort_core.c \
				src/algorithms/medium/chunk_sort_rotations.c \
				src/algorithms/medium/chunk_sort_utils.c \
				src/algorithms/simple/insertion_sort.c \
				src/algorithms/simple/insertion_utils.c \
				src/algorithms/adaptive/compute_disorder.c \
				src/algorithms/adaptive/strategy_label.c \
				src/algorithms/adaptive/dispatch.c \
				src/benchmark/bench_init.c \
				src/benchmark/bench_print.c \
				src/algorithms/small/small.c
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
