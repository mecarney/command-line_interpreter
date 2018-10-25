/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjacques <mjacques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/15 17:42:49 by mjacques          #+#    #+#             */
/*   Updated: 2018/10/24 17:51:08 by mjacques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

_Bool		ft_builtin_exit(char **ptr)
{
	ft_ptrdel(g_envp);
	g_envp = NULL;
	ft_ptrdel(ptr);
	ptr = NULL;
	ft_printf("%s%sHave a nice day!%s\n", BOLD, YELLOW, REGULAR);
	exit(0);
	return (0);
}

static void	ft_putstrecho(char *str)
{
	int i;

	i = -1;
	while (str[++i])
		(str[i] == '\'') ? NULL : ft_putchar(str[i]);
}

_Bool		ft_builtin_echo(char **ptr)
{
	int	i;

	if (ptr[1])
	{
		i = (ft_strcmp(ptr[1], "-n")) ? 0 : 1;
		while (ptr[++i])
		{
			ft_putstrecho(ptr[i]);
			(ptr[i + 1]) ? ft_putchar(' ') : NULL;
		}
		(!ft_strcmp(ptr[1], "-n")) ? NULL : ft_putchar('\n');
	}
	else
		ft_putendl("");
	return (0);
}

_Bool		ft_builtin_cd(char **ptr)
{
	int		j;
	int		ret;
	char	*str;

	ret = 1;
	if (!ptr[1])
	{
		j = ft_envar("HOME");
		str = (j != -1) ? ft_strdup(&g_envp[j][5]) : NULL;
		ret = ft_change_dir(str, 1);
	}
	else if (ptr[1] && !ptr[2])
	{
		j = ft_envar("OLDPWD");
		if (!ft_strcmp(ptr[1], "-"))
			str = (j != -1) ? ft_strdup(&g_envp[j][7]) : NULL;
		else
			str = ft_strdup(ptr[1]);
		ret = ft_change_dir(str, 0);
	}
	else
		ft_putendl("Usage: cd [directory]");
	return (ret);
}
