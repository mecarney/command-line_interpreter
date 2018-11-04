/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokens_tools.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjacques <mjacques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/27 16:23:43 by mjacques          #+#    #+#             */
/*   Updated: 2018/11/04 13:38:03 by mjacques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int			ft_check_operator(char *str)
{
	int i;

	i = -1;
	while (++i < NBRTOKENS)
		if (ft_strcmp(str, g_operator[i].value) == 0)
			return (i);
	return (-1);
}

char		*ft_tokens_val(t_ast *tokens)
{
	if (ft_check_operator(tokens->val) == -1)
		return (tokens->val);
	return (ft_tokens_val(tokens->l_child));
}

_Bool		ft_tokens_file(int fd)
{
	char	*line;

	line = NULL;
	while (get_next_line(fd, &line) > 0)
	{
		ft_putendl(line);
		ft_strdel(&line);
	}
	return (0);
}

static int	ft_get_document(char *word, int *fd)
{
	char	*line;
	char	*text;

	text = NULL;
	while (42)
	{
		if (get_next_line(0, &line) > 0)
		{
			text = (!text) ? ft_strnew(0) : free_append(text, '\n');
			if (ft_strcmp(word, line) == 0)
				break ;
			text = free_join(text, line);
			ft_strdel(&line);
		}
	}
	ft_strdel(&line);
	text = ft_expand(text, 0);
	ft_dup_fd(fd[0], fd[1], 1);
	ft_putstr(text);
	ft_strdel(&text);
	return (0);
}

void		ft_tokens_here_doc(t_ast *tokens, _Bool *ret)
{
	int		fd[2];
	int		stat_loc;
	pid_t	child_left;
	pid_t	child_right;

	pipe(fd);
	if ((child_left = fork()) == 0)
		exit(ft_get_document(ft_tokens_val(tokens->r_child), fd));
	else if (child_left < 0)
		ft_putendl("ERROR: fork() failed");
	if ((child_right = fork()) == 0)
	{
		ft_dup_fd(fd[1], fd[0], 0);
		exit(ft_tokens_exec(tokens->l_child));
	}
	else if (child_right < 0)
		ft_putendl("ERROR: fork() failed");
	close(fd[0]);
	close(fd[1]);
	waitpid(child_left, 0, WUNTRACED);
	waitpid(child_right, &stat_loc, WUNTRACED);
	*ret = stat_loc;
}
