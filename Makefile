# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mjacques <mjacques@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/07/09 11:32:00 by mjacques          #+#    #+#              #
<<<<<<< HEAD
#    Updated: 2018/10/27 12:48:17 by fuhong           ###   ########.fr        #
=======
#    Updated: 2018/10/27 16:50:13 by mjacques         ###   ########.fr        #
>>>>>>> 5d65a5a4448d94c5160749dc6e1f28d301b11d72
#                                                                              #
# **************************************************************************** #

CC			= gcc
RM			= rm -rf
NAME		= 42sh
CFLAGS		= -Wall -Wextra -Werror -g #-fsanitize=address
LIBFT		= libft/libft.a
INCLUDES	= -I includes/\
			  -I libft/header

FUNCTIONS	= main.c\
			  ft_envptools.c\
			  ft_builtins.c\
			  ft_environment.c\
			  ft_helper.c\
			  ft_command.c\
			  ft_history.c\
			  ft_historytools.c\
			  ft_check_history.c\
			  ft_tokens_exec.c\
			  ft_tokens_fct.c\
			  ft_tokens_tools.c\
			  ft_validate.c\
			  ft_tokenizer.c\
			  ft_ast.c

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
