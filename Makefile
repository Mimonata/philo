# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: spitul <spitul@student.42berlin.de>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/14 21:17:34 by spitul            #+#    #+#              #
#    Updated: 2024/11/29 06:36:23 by spitul           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= philo

CC		= cc 
CFLAGS	= -Wall -Wextra -Werror
RM		= rm -f

SRC		= main.c threading.c utils.c

OBJDIR	= ./obj

OBJ		= $(addprefix $(OBJDIR)/, $(SRC:.c=.o))

all: $(NAME)

$(OBJDIR)/%.o: %.c
		mkdir -p $(OBJDIR)
		$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
		$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean: rm -f $(OBJ)

fclean: clean
		rm -rf $(OBJDIR)
		$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
