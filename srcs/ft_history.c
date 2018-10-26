/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_history.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhong <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/25 21:22:23 by fhong             #+#    #+#             */
/*   Updated: 2018/10/26 15:27:25 by fhong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int			ft_isnumber(char *str)
{
	if (*str == '-')
		str++;
	while (*str)
		if (!ft_isdigit(*str++))
			return (0);
	return (1);
}
void		history_help(void)
{
	ft_putendl("Usage: history [flag][number]");
	ft_putendl(" history : Show the whole history list");
	ft_putendl(" history [n] : Show the last [n] history list");
	ft_putendl(" history -c  : Clear the history list");
	ft_putendl(" history -a  : Append history list to .42sh_history");
	ft_putendl(" history -r  : Append .42sh_history to history list");
	ft_putendl(" history -h  : help");
}

static int	history_flag(char **ptr)
{
	int		fd;
	char 	*line;

	if (ptr[1][2] != '\0'|| !ft_strchr("char", ptr[1][1]))
	{
		history_help();
		return (1);
	}
	else if (ptr[1][1] == 'c')
		ft_free_history();
	else if (ptr[1][1] == 'a')
		ft_write_history();
	else if (ptr[1][1] == 'r')
	{
		fd = open(".42sh_history", O_RDONLY | O_CREAT, 0666);
		int j = 0;
		while (get_next_line(fd, &line) > 0 &&  ++j < 10)
		{
			ft_history_add(line);
			ft_strdel(&line);
		}
		close(fd);
	}
	else if (ptr[1][1] == 'h')
		history_help();
	return (0);
}

_Bool		ft_builtin_history(char **ptr)
{
	if (ptr[0] && !ptr[1])
		ft_print_history(g_history, g_history->index, 1);
	else if (ptr[1] && ft_isnumber(ptr[1]) && !ptr[2] && ft_atoi(ptr[1]) > 0)
		ft_print_history(g_history, ft_atoi(ptr[1]), 1);
	else if (ptr[1] && !ptr[2] && ptr[1][0] == '-')
		return (history_flag(ptr));
	else
	{
		ft_putendl("Usage: history [flag][number] or history -h");
		return (1);
	}
	return (0);
}
