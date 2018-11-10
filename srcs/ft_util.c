/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_util.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcarney <mcarney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/05 13:30:59 by mcarney           #+#    #+#             */
/*   Updated: 2018/11/09 16:15:05 by mcarney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		count_backslashes(int index, char *str)
{
	int				i;

	i = index - 1;
	while (i >= 0 && str[i] == '\\')
		i--;
	return ((index - i + 1) % 2);
}

int		ft_strfind(const char *s1, const char *s2)
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

void	free_ast(t_ast *tokens)
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

int		ft_len_cmd(t_ast *tokens)
{
	int				len;

	len = 0;
	while (tokens)
	{
		len++;
		tokens = tokens->l_child;
	}
	return (len);
}

void	ft_check_expand(t_ast *tokens)
{
	t_ast	*tmp;

	tmp = tokens;
	while (tmp)
	{
		if ((*tmp->val) && tmp->expand == 2)
			tmp->val = get_backquote(tmp->val);
		else if ((*tmp->val) && tmp->expand == 1)
			tmp->val = ft_expand(tmp->val, 1);
		tmp = tmp->l_child;
	}
}
