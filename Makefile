# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/14 21:17:34 by spitul            #+#    #+#              #
#    Updated: 2025/01/21 19:02:28 by spitul           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= philo

CC		= gcc 
CFLAGS	= -g -pthread -Wall -Wextra -Werror #-fsanitize=thread
RM		= rm -f

SRC		= handling_init_data.c main.c monitor.c threading.c utils.c utils_parsing.c

OBJDIR	= ./obj

OBJ		= $(addprefix $(OBJDIR)/, $(SRC:.c=.o))

all: $(NAME)

$(OBJDIR)/%.o: %.c
		mkdir -p $(OBJDIR)
		$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
		$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean: 
		rm -f $(OBJ)

fclean: clean
		rm -rf $(OBJDIR)
		rm $(NAME)

re: fclean all

.PHONY: all clean fclean re
