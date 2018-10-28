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

int				is_whitespace(char c)
{
	if (c == ' ' || c == '\t' || c == '\v' || c == '\0')
		return (1);
	return (0);
}

char		*get_history(char *cmd)
{
	int				i;
	int				index;
	t_history	*tmp;

	i = -1;
	if (cmd[0] == '!' && !is_whitespace(cmd[1]))
	{
		index = (cmd[1] == '!') ? -1 : ft_atoi(&cmd[1]);
		(g_history && index < 0) && (index = g_history->index + index + 1);
		if (!g_history || index <= 0 || index > g_history->index)
		{
			ft_printf("42sh: event not found: %s\n", cmd);
			return (NULL);
		}
		tmp = g_history;
		while (tmp->index != index)
			tmp = tmp->next;
		return (tmp->command);
	}
	return ("!");
}

char	*ft_check_history_helper(char *current, char *mark, _Bool *flag)
{
	if (!ft_strncmp(mark, "!!", 2))
		(current = mark + 2) && (*flag = 1);
	else if (ft_numlen(&mark[1]) != 0)
		(current = mark + ft_numlen(&mark[1]) + 1) && (*flag = 1);
	else
		current = mark + 1;
	return (current);
}

char  *ft_check_history(char *str)
{
  char  *current;
  char  *mark;
	char	*tmp;
  char  *cmd;
	_Bool	flag;

  if (!ft_strchr(str, '!'))
		return (str);
	flag = 0;
	cmd = NULL;
	current = str;
  while (current && (mark = ft_strchr(current, '!')))
  {
		tmp = ft_strsub(current, 0, mark - current);
		cmd = (cmd) ? free_join(cmd, tmp) : ft_strdup(tmp);
		(tmp) ? ft_strdel(&tmp) : 0;
		tmp = get_history(mark);
    cmd = (cmd) ? free_join(cmd, tmp) : ft_strdup(tmp);
		current = ft_check_history_helper(current, mark, &flag);
  }
  cmd = free_join(cmd, current);
	(flag) ? (ft_putendl(cmd)) : 0;
  return (cmd);
}
