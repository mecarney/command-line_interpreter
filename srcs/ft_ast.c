/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ast.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcarney <mcarney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/26 15:54:59 by mcarney           #+#    #+#             */
/*   Updated: 2018/11/11 09:11:14 by mcarney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void		free_ast(t_ast *tokens)
{
	t_ast			*tmp;

	if (!(tokens))
		return ;
	if (!(tokens->r_child))
	{
		while (tokens->l_child)
		{
			tmp = tokens;
			tokens = tokens->l_child;
			ft_strdel(&tmp->val);
			free(tmp);
		}
	}
	else
	{
		if (tokens->l_child)
			free_ast(tokens->l_child);
		if (tokens->r_child)
			free_ast(tokens->r_child);
	}
	ft_strdel(&tokens->val);
	free(tokens);
}

int			ft_strfind(const char *s1, const char *s2)
{
	size_t			i;
	size_t			j;
	unsigned char	*str1;
	unsigned char	*str2;

	i = 0;
	j = 0;
	if (ft_strlen(s1) > ft_strlen(s2))
		return (0);
	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	while (str1[i] && str2[j])
	{
		if (str1[i] == str2[j])
		{
			i++;
			j++;
		}
		else
		{
			j = j - i + 1;
			i = 0;
		}
	}
	return (!(str1[i]));
}

t_ast		*search(t_ast **tokens, int *n, char *str, size_t len)
{
	t_ast			*old;
	t_ast			*prev;

	old = *tokens;
	prev = NULL;
	while (old)
	{
		if (old->operator && ft_strlen(old->val) >= len &&\
			ft_strfind(old->val, str))
		{
			if (prev)
				prev->l_child = NULL;
			else if (!(ft_strcmp(old->val, "<")) ||\
					!(ft_strcmp(old->val, ">")) || !(ft_strcmp(old->val, "<<")))
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
	t_ast			*old;
	t_ast			*prev;
	int				n;

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
