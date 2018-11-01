/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_history.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhong <fhong@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/25 21:22:23 by fhong             #+#    #+#             */
/*   Updated: 2018/10/31 17:25:52 by fhong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

char		*g_history_file;

int			ft_numlen(char *str)
{
	int	len;

	len = -1;
	if (str[len + 1] == '-')
		len++;
	if (!ft_isdigit(str[len + 1]))
		return (0);
	while (str[++len])
		if (!ft_isdigit(str[len]))
			break ;
	return (len);
}

static int	history_help(int i)
{
	ft_putendl("Usage: history [flag][number]");
	ft_putendl(" history : Show the whole history list");
	ft_putendl(" history [n] : Show the last [n] history list");
	ft_putendl(" history -c  : Clear the history list");
	ft_putendl(" history -a  : Append history list to .42sh_history");
	ft_putendl(" history -r  : Append .42sh_history to history list");
	ft_putendl(" history -h  : help");
	return (i);
}

char		*get_history_file(void)
{
	int		j;
	char	*str;

	j = ft_envar("HOME");
	if (j != -1)
	{
		str = ft_strdup(&g_envp[j][5]);
		if (str[ft_strlen(str) - 1] == '/')
			str = free_join(str, ".42sh_history");
		else
			str = free_join(str, "/.42sh_history");
		return (str);
	}
	return (NULL);
}

static int	history_flag(char **ptr)
{
	int		fd;
	char	*line;

	if (ptr[1][2] != '\0' || !ft_strchr("char", ptr[1][1]))
		return (history_help(1));
	else if (ptr[1][1] == 'c')
		ft_free_history();
	else if (ptr[1][1] == 'a')
		ft_write_history();
	else if (ptr[1][1] == 'r')
	{
		fd = open(g_history_file, O_RDONLY | O_CREAT, 0666);
		while (get_next_line(fd, &line) > 0)
		{
			ft_history_add(line);
			ft_strdel(&line);
		}
		close(fd);
	}
	else if (ptr[1][1] == 'h')
		history_help(0);
	return (0);
}

_Bool		ft_builtin_history(char **ptr)
{
	if (ptr[0] && !ptr[1])
		ft_print_history(g_history, g_history->index, 1);
	else if (ptr[1] && ft_numlen(ptr[1]) && !ptr[2] && ft_atoi(ptr[1]) > 0)
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
