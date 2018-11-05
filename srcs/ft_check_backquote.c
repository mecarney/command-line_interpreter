/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_backquote.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhong <fhong@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/30 21:19:54 by fhong             #+#    #+#             */
/*   Updated: 2018/11/04 16:37:07 by mjacques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static char	*get_pipe_stdin(int *fd)
{
	int		fd_base;
	char	*line;
	char	*new_str;

	new_str = NULL;
	fd_base = dup(0);
	ft_dup_fd(fd[1], fd[0], 0);
	while (get_next_line(0, &line) > 0)
	{
		if (new_str)
		{
			new_str = free_append(new_str, ' ');
			new_str = free_join(new_str, line);
		}
		else
			new_str = ft_strdup(line);
		ft_strdel(&line);
	}
	dup2(fd_base, 0);
	return (new_str);
}

char		*get_backquote(char *str)
{
	int		stat_loc;
	int		fd[2];
	_Bool	ret;
	pid_t	pid;

	pipe(fd);
	if ((pid = fork()) == 0)
	{
		ft_dup_fd(fd[0], fd[1], 1);
		ft_run_shell(&ret, str);
		exit(1);
	}
	else if (pid < 0)
		ft_putendl("ERROR: fork() failed");
	waitpid(pid, &stat_loc, WUNTRACED);
	ft_strdel(&str);
	return (get_pipe_stdin(fd));
}
