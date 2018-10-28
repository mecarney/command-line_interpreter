/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokens_fct.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjacques <mjacques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/25 13:01:24 by mjacques          #+#    #+#             */
/*   Updated: 2018/10/27 16:33:42 by mjacques         ###   ########.fr       */
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
	int		fd_new;
	int		fd_base;
	char	*file;

	fd_base = dup(1);
	file = ft_tokens_val(tokens->r_child);
	fd_new = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (fd_new == -1)
		*ret = ft_return_access("42sh", tokens->r_child->val);
	else
	{
		dup2(fd_new, 1);
		*ret = ft_tokens_exec(tokens->l_child);
		close(fd_new);
		dup2(fd_base, 1);
	}
}

void		ft_tokens_redirect_append(t_ast *tokens, _Bool *ret)
{
	int		fd_new;
	int		fd_base;
	char	*file;

	fd_base = dup(1);
	file = ft_tokens_val(tokens->r_child);
	fd_new = open(file, O_WRONLY | O_CREAT, 0666);
	if (fd_new == -1)
		*ret = ft_return_access("42sh", tokens->r_child->val);
	else
	{
		dup2(fd_new, 1);
		lseek(fd_new, 0, SEEK_END);
		*ret = ft_tokens_exec(tokens->l_child);
		close(fd_new);
		dup2(fd_base, 1);
	}
}

void		ft_tokens_redirect_fd(t_ast *tokens, _Bool *ret)
{
	int		file;
	int		std_out;
	int		stat_loc;
	pid_t	child;

	if ((file = open(ft_tokens_val(tokens->r_child), O_RDONLY)) == -1)
		*ret = ft_return_access("42sh", tokens->r_child->val);
	else
	{
		std_out = dup(0);
		dup2(file, 0);
		if ((child = fork()) == 0)
			exit(ft_tokens_exec(tokens->l_child));
		else if (child < 0)
			ft_putendl("ERROR: fork() failed");
		close(file);
		waitpid(child, &stat_loc, WUNTRACED);
		dup2(std_out, 0);
		*ret = stat_loc;
	}
}
