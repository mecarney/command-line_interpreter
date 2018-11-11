/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_here_document.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjacques <mjacques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/05 15:40:18 by mjacques          #+#    #+#             */
/*   Updated: 2018/11/11 13:01:18 by mjacques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static char	*ft_get_backquote(char *str)
{
	char	*current;
	char	*mark;
	char	*tmp;
	char	*cmd;

	if (!str || !ft_strchr(str, '`'))
		return (str);
	cmd = NULL;
	current = str;
	while (current && (mark = ft_strchr(current, '`')))
	{
		tmp = ft_strsub(current, 0, mark - current);
		cmd = (cmd) ? free_join(cmd, tmp) : ft_strdup(tmp);
		(tmp) ? ft_strdel(&tmp) : 0;
		tmp = ft_strsub(mark, 1, ft_strchr(&mark[1], '`') - mark - 1);
		tmp = get_backquote(tmp, '\n');
		cmd = (cmd) ? free_join(cmd, tmp) : ft_strdup(tmp);
		current = ft_strchr(&mark[1], '`') + 1;
		(tmp) ? ft_strdel(&tmp) : 0;
	}
	cmd = free_join(cmd, current);
	ft_strdel(&str);
	return (cmd);
}

static int	ft_get_document(char *word, int *fd)
{
	char	*line;
	char	*document;

	document = NULL;
	while (42)
	{
		ft_putstr_fd("> ", 0);
		if (get_next_line(0, &line) > 0)
		{
			document = (!document) ? ft_strnew(0) : free_append(document, '\n');
			if (ft_strcmp(word, line) == 0)
				break ;
			document = free_join(document, line);
			ft_strdel(&line);
		}
	}
	ft_strdel(&line);
	document = ft_expand(document, 0);
	document = ft_get_backquote(document);
	ft_dup_fd(fd[0], fd[1], 1);
	ft_putstr(document);
	ft_strdel(&document);
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
