/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_command.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjacques <mjacques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/17 23:58:44 by mjacques          #+#    #+#             */
/*   Updated: 2018/10/25 17:36:31 by mjacques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static char	**ft_pathtostr(void)
{
	int		i;
	char	*path;
	char	**directory;

	i = ft_envar("PATH");
	path = (i != -1) ? ft_strdup(&g_envp[i][5]) : NULL;
	if (!path)
		return (NULL);
	directory = ft_strsplit(path, ':');
	ft_strdel(&path);
	i = -1;
	while (directory && directory[++i])
		directory[i] = free_append(directory[i], '/');
	return (directory);
}

static int	ft_check_builtin(char **cmd)
{
	int ret;

	ret = -1;
	while (++ret < NBRBUILTIN)
		if (ft_strcmp(cmd[0], g_builtin[ret].command) == 0)
			return (ret);
	return (-1);
}

static char	*ft_check_path(char **ptr, int *ret)
{
	int		i;
	char	*tmp;
	char	**path;

	path = ft_pathtostr();
	i = -1;
	while (path && path[++i])
	{
		tmp = ft_strjoin(path[i], ptr[0]);
		if ((*ret = access(tmp, F_OK)) == 0)
			break ;
		ft_strdel(&tmp);
	}
	ft_ptrdel(path);
	return (tmp);
}

static void	ft_execute_cmd(char *path, char **ptr, int *ret)
{
	pid_t	pid;

	if ((pid = fork()) == 0)
		execve(path, ptr, g_envp);
	else if (pid < 0)
		ft_putendl("ERROR: Fail to create new process");
	waitpid(pid, ret, WUNTRACED);
	(ft_strcmp(path, ptr[0])) ? ft_strdel(&path) : NULL;
}

int			ft_run_cmd(char **cmd)
{
	int		ret;
	char	*path;

	ret = 0;
	if (cmd == NULL)
		return (ret);
	if ((ret = ft_check_builtin(cmd)) != -1)
		ret = g_builtin[ret].fct(cmd);
	else
	{
		if ((ret = access(cmd[0], X_OK)) == 0  && ft_strchr(cmd[0], '/'))
			path = cmd[0];
		else
			path = ft_check_path(cmd, &ret);
		if (ret == 0)
			ft_execute_cmd(path, cmd, &ret);
		else
		{
			if (access(cmd[0], F_OK & R_OK) == 0 && ft_strchr(cmd[0], '/'))
				ft_printf("42sh: permission denied: %s\n", cmd[0]);
			else
				ft_printf("42sh: command not found: %s\n", cmd[0]);
		}
	}
	return (ret);
}
