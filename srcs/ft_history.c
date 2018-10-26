/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_history.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fuhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/25 18:34:18 by fuhong            #+#    #+#             */
/*   Updated: 2018/10/25 19:22:40 by fuhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_history			*g_history;
static t_history	*new_history(char *command)
{
	t_history	*new_hist;

	new_hist = (t_history *)malloc(sizeof(t_history) * 1);
	new_hist->index = 0;
	new_hist->command = ft_strdup(command);
	new_hist->next = NULL;
	return (new_hist);
}

void	ft_history_add(char *command)
{
	int			i;
	t_history	*tmp;
	t_history	*new_hist;

	i = 1;
	tmp = g_history;
	new_hist = new_history(command);
	if (!g_history)
	{
		g_history = new_hist;
		new_hist->index = 1;
	}
	else
	{
		while (tmp->next && ++i)
			tmp = tmp->next;
		tmp->next = new_hist;
		new_hist->index = i + 1;
	}
}

void	ft_print_history(void)
{
	t_history *tmp;

	tmp = g_history;
//	ft_printf("--------- history ---------\n");
	while (tmp)
	{
		ft_printf("[%4d] %s\n", tmp->index, tmp->command);
		tmp = tmp->next;
	}
}
