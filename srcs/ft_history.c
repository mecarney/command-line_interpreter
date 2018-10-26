/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_history.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhong <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/25 21:22:23 by fhong             #+#    #+#             */
/*   Updated: 2018/10/25 22:51:06 by fhong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		ft_isnumber(char *str)
{
	while (*str)
		if (!ft_isdigit(*str++))
			return (0);
	return (1);
}

/*
static	int	check_histroy_para(**ptr)
{
	int i;

	i = 0;
	while (ptr[++i])
	{
		
	}
}*/

_Bool		ft_builtin_history(char **ptr)
{
	int		ret;

	(void)ptr;
	ret = 1;
	if (ptr[1] && !ft_isnumber(ptr[1]))
	{
		ft_putendl("Usage: history [flag][number]");
		return (1);
	}
	if (ptr[1])
		ft_print_history(g_history, ft_atoi(ptr[1]), 1);
	else
		ft_print_history(g_history, g_history->index, 1);
	return (0);
}
