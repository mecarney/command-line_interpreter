# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mjacques <mjacques@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/07/09 11:32:00 by mjacques          #+#    #+#              #
#    Updated: 2018/10/25 15:19:00 by mjacques         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC			= gcc
RM			= rm -rf
NAME		= 42sh
CFLAGS		= -Wall -Wextra -Werror -g
LIBFT		= libft/libft.a
INCLUDES	= -I includes/\
			  -I libft/header

FUNCTIONS	= main.c\
			  ft_envptools.c\
			  ft_builtins.c\
			  ft_environement.c\
			  ft_helper.c\
			  ft_command.c\
			  lexer_parser.c\
			  ft_tokens_exec.c\
			  ft_tokens_fct.c

SRCPATH		= srcs/
FILES		= $(addprefix $(SRCPATH), $(FUNCTIONS))
OBJECTS		= $(FILES:.c=.o)

.PHONY: all clean fclean re

all: $(NAME)

$(SRCPATH)%.o: $(SRCPATH)%.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(OBJECTS) $(LIBFT)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJECTS) $(LIBFT)
	@echo "\033[32m42sh compiled!\033[0m"

$(LIBFT):
	@make -C libft/

clean:
	@$(RM) $(OBJECTS)
	@make clean -C libft/

fclean: clean
	@$(RM) $(NAME)
	@make fclean -C libft/
	@echo "\033[31m42sh cleaned!\033[0m"

re: fclean all
