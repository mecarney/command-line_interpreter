/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokens_fct.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjacques <mjacques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/25 13:01:24 by mjacques          #+#    #+#             */
/*   Updated: 2018/10/25 15:21:05 by mjacques         ###   ########.fr       */
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

	fd_base = dup(1);
	fd_new = open(tokens->r_child->val, O_RDWR | O_CREAT | O_TRUNC, 0666);
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

	fd_base = dup(1);
	fd_new = open(tokens->r_child->val, O_RDWR | O_CREAT, 0666);
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
	(void)tokens;
	(void)ret;
	ft_putendl("Attend, Wait! : Redirect Fd");
}
