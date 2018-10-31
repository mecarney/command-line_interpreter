# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mjacques <mjacques@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/07/09 11:32:00 by mjacques          #+#    #+#              #
#    Updated: 2018/10/31 15:43:02 by fhong            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC			= gcc
RM			= rm -rf
NAME		= 42sh
CFLAGS		= -Wall -Wextra -Werror -g
LIBFT		= libft/libft.a
INCLUDES	= -I includes/\
			  -I libft/header

BASE		= main.c
BUILTINS	= ft_builtins.c ft_envp.c ft_envp_tools.c ft_helper.c
EXECUTE		= ft_command.c ft_tokens_exec.c ft_tokens_fct.c ft_tokens_tools.c
PARSER		= ft_validate.c ft_tokenizer.c ft_ast.c ft_check_backquote.c
HISTORY		= ft_history.c ft_historytools.c ft_check_history.c
FUNCTIONS	= $(BASE) $(BUILTINS) $(EXECUTE) $(PARSER) $(HISTORY)

FILES		= $(addprefix srcs/, $(FUNCTIONS))
OBJECTS		= $(FILES:.c=.o)

.PHONY: all clean fclean re

all: $(NAME)

%.o: %.c
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
