/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokens_fct.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjacques <mjacques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/25 13:01:24 by mjacques          #+#    #+#             */
/*   Updated: 2018/11/04 18:26:46 by mjacques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void		ft_tokens_and(t_ast *tokens, _Bool *ret)
{
	if ((*ret = ft_tokens_exec(tokens->l_child)) == 0)
		*ret = ft_tokens_exec(tokens->r_child);
}

void		ft_tokens_or(t_ast *tokens, _Bool *ret)
{
	if ((*ret = ft_tokens_exec(tokens->l_child)) == 1)
		*ret = ft_tokens_exec(tokens->r_child);
}

void		ft_tokens_redirect(t_ast *tokens, _Bool *ret)
{
	int			fd_new;
	int			fd_base;
	char		*file;
	static char	*cat[] = {"cat", 0};

	fd_new = 1;
	fd_base = dup(1);
	if ((file = ft_tokens_val(tokens->r_child)))
		fd_new = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (fd_new == -1)
		*ret = ft_return_access("42sh", file);
	else
	{
		dup2(fd_new, 1);
		if (tokens->l_child)
			*ret = ft_tokens_exec(tokens->l_child);
		else
			*ret = ft_run_cmd(cat);
		close(fd_new);
		dup2(fd_base, 1);
	}
}

void		ft_tokens_redirect_append(t_ast *tokens, _Bool *ret)
{
	int			fd_new;
	int			fd_base;
	char		*file;
	static char	*cat[] = {"cat", 0};

	fd_new = 1;
	fd_base = dup(1);
	if ((file = ft_tokens_val(tokens->r_child)))
		fd_new = open(file, O_WRONLY | O_CREAT, 0666);
	if (fd_new == -1)
		*ret = ft_return_access("42sh", file);
	else
	{
		dup2(fd_new, 1);
		lseek(fd_new, 0, SEEK_END);
		if (tokens->l_child)
			*ret = ft_tokens_exec(tokens->l_child);
		else
			*ret = ft_run_cmd(cat);
		close(fd_new);
		dup2(fd_base, 1);
	}
}

void		ft_tokens_redirect_fd(t_ast *tokens, _Bool *ret)
{
	int		fd_new;
	int		std_out;
	int		stat_loc;
	pid_t	child;

	if ((fd_new = open(ft_tokens_val(tokens->r_child), O_RDONLY)) == -1)
		*ret = ft_return_access("42sh", ft_tokens_val(tokens->r_child));
	else
	{
		if (!tokens->l_child)
			*ret = ft_tokens_file(fd_new);
		else
		{
			std_out = dup(0);
			dup2(fd_new, 0);
			if ((child = fork()) == 0)
				exit(ft_tokens_exec(tokens->l_child));
			else if (child < 0)
				ft_putendl("ERROR: fork() failed");
			close(fd_new);
			waitpid(child, &stat_loc, WUNTRACED);
			dup2(std_out, 0);
			*ret = stat_loc;
		}
	}
}
