/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjacques <mjacques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/15 14:15:18 by mjacques          #+#    #+#             */
/*   Updated: 2018/10/26 20:23:39 by fhong            ###   ########.fr       */
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
	{"exit", &ft_builtin_exit},
	{"history", &ft_builtin_history}
};

int		main(int argc, char **argv, char **envp)
{
	_Bool		ret;

	(argc != 1 || !argv[0]) ? ft_error("Usage: ./42sh") : NULL;
	ft_printf("%s%sWelcome!%s Make great code today\n", BOLD, BLUE, REGULAR);
	envp = ft_newenv(envp);
	ret = 0;
	g_history = NULL;
	while (42)
		ft_init(&ret);
	return (0);
}

void	ft_init(_Bool *ret)
{
	char	*line;
	t_okenize				t;
	t_ast			*tokens;

	(*ret) ? ft_putstr(RED) : ft_putstr(PURPLE);
	ft_putstr(SHELLNAME);
	if (get_next_line(0, &line) <= 0)
		return ;
	t.i = -1;
	t.j = 0;
	t.tokens = 0;
	t.prev = '\0';
	tokens = NULL;
	(line) ? check_quotes(line, &t, &tokens) : 0;
	tokens = parser(&tokens, NULL);
	*ret = ft_tokens_exec(tokens);
	// ft_strdel(&line); I free it in "check_quotes()" and "ft_check_history"
	free_ast(tokens);
}
