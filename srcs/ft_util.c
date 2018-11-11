/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_util.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcarney <mcarney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/05 13:30:59 by mcarney           #+#    #+#             */
/*   Updated: 2018/11/11 09:06:02 by mcarney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

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
		if (!(tmp->val))
			ft_restart(tokens, "Null expansion");
		tmp = tmp->l_child;
	}
}
