/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_history.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fuhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/25 18:34:18 by fuhong            #+#    #+#             */
/*   Updated: 2018/10/25 20:39:57 by fhong            ###   ########.fr       */
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
	t_history	*new_hist;

	i = 1;
	if (!command || !*command)
		return ;
	new_hist = new_history(command);
	if (!g_history)
	{
		g_history = new_hist;
		new_hist->index = 1;
	}
	else
	{
		new_hist->next = g_history;
		new_hist->index = g_history->index + 1;
		g_history = new_hist;
	}
}

void	ft_print_history(t_history *history)
{
	t_history	*tmp;

	tmp = (history == g_history) ? g_history : history;
	if (tmp->next)
		ft_print_history(tmp->next);
	ft_printf("[%4d] %s\n", tmp->index, tmp->command);
}
