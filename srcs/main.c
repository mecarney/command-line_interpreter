/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjacques <mjacques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/15 14:15:18 by mjacques          #+#    #+#             */
/*   Updated: 2018/10/31 16:48:35 by mjacques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

char		**g_envp;
char		*g_history_file;
t_builtin	g_builtin[NBRBUILTIN] = {
	{"cd", &ft_builtin_cd},
	{"echo", &ft_builtin_echo},
	{"setenv", &ft_builtin_setenv},
	{"unsetenv", &ft_builtin_unsetenv},
	{"env", &ft_builtin_env},
	{"exit", &ft_builtin_exit},
	{"history", &ft_builtin_history}
};

void	ft_run_shell(_Bool *ret, char *line)
{
	t_okenize	t;
	t_ast		*tokens;

	t.i = -1;
	t.j = 0;
	t.tokens = 0;
	t.prev = '\0';
	tokens = NULL;
	check_quotes(line, &t, &tokens);
	tokens = parser(&tokens, NULL);
	*ret = ft_tokens_exec(tokens);
	free_ast(tokens);
}

int		main(int argc, char **argv, char **envp)
{
	char	*line;
	_Bool	ret;

	(argc != 1 || !argv[0]) ? ft_error("Usage: ./42sh") : NULL;
	ft_printf("%s%sWelcome!%s Make great code today\n", BOLD, BLUE, REGULAR);
	envp = ft_newenv(envp);
	g_history = NULL;
	g_history_file = get_history_file();
	ret = 0;
	while (42)
	{
		(ret) ? ft_putstr(RED) : ft_putstr(PURPLE);
		ft_putstr(SHELLNAME);
		if (get_next_line(0, &line) > 0)
		{
			ft_run_shell(&ret, line);
			ft_strdel(&line);
		}
	}
	return (0);
}
