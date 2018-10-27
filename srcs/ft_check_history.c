/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_history.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhong <fhong@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/26 16:21:52 by fhong             #+#    #+#             */
/*   Updated: 2018/10/26 21:44:51 by fhong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

char		*get_history(char *cmd)
{
	int				i;
	int				index;
	t_history	*tmp;

	i = -1;
	if (cmd[0] == '!')
	{
		index = (cmd[1] == '!') ? -1 : ft_atoi(&cmd[1]);
		(g_history && index < 0) && (index = g_history->index + index + 1);
		if (!g_history || index <= 0 || index > g_history->index)
		{
			ft_printf("sh: %s: event not found\n", cmd);
			return (NULL);
		}
		tmp = g_history;
		while (tmp->index != index)
			tmp = tmp->next;
		return (tmp->command);
	}
	return (NULL);
}

char  *ft_check_history(char *str)
{
  char  *current;
  char  *mark;
	char	*tmp;
  char  *cmd;

  if (!ft_strchr(str, '!'))
    return (str);
  current = str;
	cmd = NULL;
  while (current && (mark = ft_strchr(current, '!')))
  {
		tmp = ft_strsub(current, 0, mark - current);
		cmd = (cmd) ? free_join(cmd, tmp) : ft_strdup(tmp);
		(tmp) ? ft_strdel(&tmp) : 0;
		tmp = get_history(mark);
    cmd = (cmd) ? free_join(cmd, tmp) : ft_strdup(tmp);
		if (!ft_strncmp(mark, "!!", 2))
			current = mark + 2;
		else
    	current += (ft_numlen(&mark[1]) + 1);
  }
  cmd = free_join(cmd, current);
  //ft_strdel(&str);
	ft_putendl(cmd);
  return (cmd);
}
