/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_command.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjacques <mjacques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/17 23:58:44 by mjacques          #+#    #+#             */
/*   Updated: 2018/10/23 19:05:02 by mjacques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static char	**ft_pathtostr(void)
{
	int		i;
	char	**path;
	char	**directory;

	path = NULL;
	i = -1;
	while (g_envp[++i])
		if (ft_strcmp(g_envp[i], "PATH") == '=')
			path = ft_strsplit(g_envp[i], '=');
	if (!path || !path[0] || !path[1])
		return (NULL);
	directory = ft_strsplit(path[1], ':');
	ft_ptrdel(path);
	i = -1;
	while (directory && directory[++i])
		directory[i] = free_append(directory[i], '/');
	return (directory);
}

static int	ft_checkpath(char **ptr)
{
	int		i;
	int		on;
	char	*tmp;
	char	**path;
	pid_t	newchild;

	path = ft_pathtostr();
	i = -1;
	on = -1;
	while (path && path[++i])
	{
		tmp = ft_strjoin(path[i], ptr[0]);
		if ((newchild = fork()) == 0)
			on = execve(tmp, ptr, g_envp);
		else
			waitpid(newchild, &on, WUNTRACED);
		ft_strdel(&tmp);
		if (on != -1)
			break ;
	}
	ft_ptrdel(path);
	return (on);
}

void		ft_run_system_cmd(char **ptr)
{
	int		on;
	pid_t	newchild;

	on = -1;
	if ((newchild = fork()) == 0)
		on = execve(ptr[0], ptr, g_envp);
	else
		waitpid(newchild, &on, WUNTRACED);
	if (on == -1)
		on = ft_checkpath(ptr);
	(on == -1) ? ft_printf("%s: command not found\n", ptr[0]) : 0;
	exit(0);
}

int			ft_check_builtin(char **cmd)
{
	int ret;

	ret = -1;
	while (++ret < NBRBUILTIN)
		if (ft_strcmp(cmd[0], g_builtin[ret].command) == 0)
			return (ret);
	return (-1);
}

void		ft_run_cmd(char **cmd)
{
	int		ret;
	int		stat_loc;
	_Bool	result;
	pid_t	pid;

	ret = -1;
	result = 0;
	if (cmd == NULL)
		return ;
	if ((ret = ft_check_builtin(cmd)) != -1)
		result = g_builtin[ret].fct(cmd);
	else
	{
		if ((pid = fork()) == 0)
			ft_run_system_cmd(cmd);
		else if (pid < 0)
			ft_error("ERROR: Fail to create new process");
		waitpid(pid, &stat_loc, WUNTRACED);
	}
}
