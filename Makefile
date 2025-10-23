# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gorg <gorg@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/24 23:25:13 by gorg              #+#    #+#              #
#    Updated: 2025/05/16 19:05:35 by gorg             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
CC = cc
CFLAGS = -O2 -Wall -Wextra -Werror

###
SRCS_DIR = ./srcs/

###
FILES = main.c cycle.c data.c time.c utils.c utils2.c utils3.c

###
SRCS = $(addprefix $(SRCS_DIR),$(FILES))
OBJS = $(SRCS:.c=.o)

###
all:  $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) 

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

###

.PHONY: all clean fclean re
