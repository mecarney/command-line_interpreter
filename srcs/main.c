/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjacques <mjacques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/15 14:15:18 by mjacques          #+#    #+#             */
/*   Updated: 2018/10/24 17:51:02 by mjacques         ###   ########.fr       */
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

	(argc != 1 || !argv[0]) ? ft_error("Usage: ./minishell") : NULL;
	ft_printf("%s%sWelcome!%s Make great code today\n", BOLD, BLUE, REGULAR);
	envp = ft_newenv(envp);
	ret = 0;
	while (!(command = NULL))
	{
		(ret) ? ft_putstr(RED) : ft_putstr(PURPLE);
		if ((command = ft_init()) && *command)
			ret = ft_run_cmd(command);
		ft_ptrdel(command);
	}
	return (0);
}

char	**ft_init(void)
{
	char	*line;
	char	**command;

	ft_putstr(BOLD);
	ft_putstr(SHELLNAME);
	ft_putstr(REGULAR);
	if (get_next_line(0, &line) <= 0)
		return (NULL);
	command = ft_strsplitspaces(line);
	command = ft_checkquote(command);
	command = ft_checkenv(command);
	ft_strdel(&line);
	return (command);
}
