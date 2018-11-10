/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ast.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcarney <mcarney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/26 15:54:59 by mcarney           #+#    #+#             */
/*   Updated: 2018/11/10 14:35:09 by mcarney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_ast		*search(t_ast **tokens, int *n, char *str, size_t len)
{
	t_ast	*old;
	t_ast	*prev;

	old = *tokens;
	prev = NULL;
	while (old)
	{
		if (old->operator && ft_strlen(old->val) >= len &&\
			ft_strfind(old->val, str))
		{
			if (prev)
				prev->l_child = NULL;
			else if (!(ft_strcmp(old->val, "<")) ||
					!(ft_strcmp(old->val, ">") || !(ft_strcmp(old->val, "<<"))))
				*tokens = NULL;
			else
				ft_restart(*tokens, "parse error");
			*n = 0;
			return (old);
		}
		prev = old;
		old = old->l_child;
	}
	return (*tokens);
}

t_ast		*parser(t_ast **tokens, t_ast *parent)
{
	t_ast	*old;
	t_ast	*prev;
	int		n;

	if (!(*tokens))
		return (NULL);
	n = 1;
	old = *tokens;
	prev = NULL;
	old = search(tokens, &n, ";", 1);
	(n) ? old = search(tokens, &n, "&&||", 2) : 0;
	(n) ? old = search(tokens, &n, "|", 1) : 0;
	(n) ? old = search(tokens, &n, "<<", 1) : 0;
	(n) ? old = search(tokens, &n, ">>", 1) : 0;
	if (n == 0)
	{
		old->r_child = parser(&old->l_child, old);
		old->l_child = parser(tokens, old);
	}
	else
		old = *tokens;
	old->parent = (parent) ? parent : NULL;
	return (old);
}
