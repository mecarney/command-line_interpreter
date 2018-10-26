/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjacques <mjacques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/15 14:15:18 by mjacques          #+#    #+#             */
/*   Updated: 2018/10/25 17:55:34 by fhong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

char		**g_envp;
t_builtin	g_builtin[NBRBUILTIN] = {
	{"cd", &ft_builtin_cd},
	{"echo", &ft_builtin_echo},
	{"setenv", &ft_builtin_setenv},
	{"unsetenv", &ft_builtin_unsetenv},
	{"env", &ft_builtin_env},
	{"exit", &ft_builtin_exit}
};

int		main(int argc, char **argv, char **envp)
{
	_Bool	ret;
	char	**command;

	(argc != 1 || !argv[0]) ? ft_error("Usage: ./42sh") : NULL;
	ft_printf("%s%sWelcome!%s Make great code today\n", BOLD, BLUE, REGULAR);
	envp = ft_newenv(envp);
	ret = 0;
	while (!(command = NULL))
	{
		(ret) ? ft_putstr(RED) : ft_putstr(PURPLE);
		ret = ft_init();
		ft_ptrdel(command);
	}
	return (0);
}

_Bool	ft_init(void)
{
	char	*line;
	_Bool	ret;
	t_okenize				t;
	t_ast			*tokens;


	ft_putstr(BOLD);
	ft_putstr(SHELLNAME);
	ft_putstr(REGULAR);
	if (get_next_line(0, &line) <= 0)
		return (NULL);
	defaults(&t);
	tokens = NULL;
	check_quotes(line, &t, &tokens);
	// printf("%d\n", t.tokens);
	tokens = parser(&tokens, NULL);
	// tokens ? printf("%s \n", tokens->val) : 0;
	// print_ast(tokens);
	ret = ft_tokens_exec(tokens);
	ft_strdel(&line);
	free_ast(tokens);
	return (ret);
}
