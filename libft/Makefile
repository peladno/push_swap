# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jperez-u <jperez-u@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/23 16:23:54 by skusakab          #+#    #+#              #
#    Updated: 2026/06/12 15:51:56 by jperez-u         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= libft.a
SRCS	=	ft_atoi.c \
			ft_bzero.c \
			ft_calloc.c \
			ft_isalnum.c \
			ft_isalpha.c \
			ft_isascii.c \
			ft_isdigit.c \
			ft_isprint.c \
			ft_itoa.c \
			ft_lstadd_back.c \
			ft_lstadd_front.c \
			ft_lstclear.c \
			ft_lstdelone.c \
			ft_lstiter.c \
			ft_lstlast.c \
			ft_lstmap.c \
			ft_lstnew.c \
			ft_lstsize.c \
			ft_memchr.c \
			ft_memcmp.c \
			ft_memcpy.c \
			ft_memmove.c \
			ft_memset.c \
			ft_putchar_fd.c \
			ft_putendl_fd.c \
			ft_putnbr_fd.c \
			ft_putstr_fd.c \
			ft_split.c \
			ft_strchr.c \
			ft_strdup.c \
			ft_striteri.c \
			ft_strjoin.c \
			ft_strlcat.c \
			ft_strlcpy.c \
			ft_strlen.c \
			ft_strmapi.c \
			ft_strncmp.c \
			ft_strnstr.c \
			ft_strrchr.c \
			ft_strtrim.c \
			ft_substr.c \
			ft_tolower.c \
			ft_toupper.c \
			ft_printft/ft_printf.c \
			ft_printft/print_char.c \
			ft_printft/print_string.c \
			ft_printft/print_pointer.c \
			ft_printft/print_hex.c \
			ft_printft/print_number.c \
			ft_printft/print_unsigned.c
OBJS	= $(SRCS:%.c=%.o)
CC		= cc
CFLAGS	= -Wall -Wextra -Werror
HEADER	= libft.h
PRINTF_HEADER = ft_printft/ft_printf.h

all:	$(NAME)

$(NAME):	$(OBJS)
	ar rcs $(NAME) $(OBJS)

%.o:	%.c
	$(CC) $(CFLAGS) -c $< -o $@

ft_printft/%.o: ft_printft/%.c $(PRINTF_HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
