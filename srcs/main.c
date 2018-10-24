/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjacques <mjacques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/15 14:15:18 by mjacques          #+#    #+#             */
/*   Updated: 2018/10/23 19:06:53 by mjacques         ###   ########.fr       */
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
	char	**command;

	(argc != 1 || !argv[0]) ? ft_error("Usage: ./minishell") : NULL;
	ft_putendl("\033[1m\033[92mWelcome!\033[0m Make great code today");
	envp = ft_newenv(envp);
	while (!(command = NULL))
	{
		if ((command = ft_init()) && *command)
			ft_run_cmd(command);
		ft_ptrdel(command);
	}
	return (0);
}

char	**ft_init(void)
{
	char	*line;
	char	**command;

	ft_putstr(SHELLNAME);
	if (get_next_line(0, &line) <= 0)
		return (NULL);
	command = ft_strsplitspaces(line);
	command = ft_checkquote(command);
	command = ft_checkenv(command);
	ft_strdel(&line);
	return (command);
}
