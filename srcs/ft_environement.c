/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_environement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjacques <mjacques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/15 17:43:29 by mjacques          #+#    #+#             */
/*   Updated: 2018/10/23 17:11:18 by mjacques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static int		ft_isvariable(char **ptr)
{
	int	i;

	i = 0;
	while (ptr[++i])
		if (!ft_strchr(ptr[i], '='))
			return (i);
	return (0);
}

_Bool			ft_builtin_env(char **ptr)
{
	int i;
	int j;
	int	on;

	if ((j = ft_isvariable(ptr)) != 0)
	{
		ft_printf("%s: %s: No such file or directory\n", ptr[0], ptr[j]);
		return (1);
	}
	i = -1;
	while (g_envp[++i] && !(j = 0))
	{
		on = 1;
		while (ptr[++j])
			if (!ft_strncmp(g_envp[i], ptr[j], ENVNAME(g_envp[i])) && !(on = 0))
				ft_putendl(ptr[j]);
		(on) ? ft_putendl(g_envp[i]) : NULL;
	}
	j = 0;
	while (ptr[++j] && (i = -1) == -1)
		(ft_envar(ptr[j]) == -1) ? ft_putendl(ptr[j]) : 0;
	return (1);
}

_Bool			ft_setenv(char *name, char *value)
{
	int		i;
	char	*str;

	str = ft_strappend(name, '=');
	if (value)
		str = free_join(str, value);
	i = ft_envar(name);
	if (i != -1)
		g_envp[i] = free_str(g_envp[i], ft_strdup(str));
	else
		g_envp = ft_addenv(str);
	ft_strdel(&str);
	return (1);
}

_Bool			ft_builtin_setenv(char **ptr)
{
	int		i;
	char	*name;

	if (ptr[1] && ft_isvariable(ptr) == 0 && !ptr[2])
	{
		name = ft_strmcpy(ptr[1], ENVNAME(ptr[1]));
		i = ft_envar(name);
		if (i != -1)
			g_envp[i] = free_str(g_envp[i], ft_strdup(ptr[1]));
		else
			g_envp = ft_addenv(ptr[1]);
		ft_strdel(&name);
	}
	else
		ft_putendl("Usage: setenv name=[value]");
	return (1);
}

_Bool			ft_builtin_unsetenv(char **ptr)
{
	int	i;

	if (!ptr[1] || (ptr[1] && ptr[2]) || !ft_isvariable(ptr))
		ft_putendl("Usage: unsetenv name");
	else
	{
		i = ft_envar(ptr[1]);
		if (i == -1)
			return (1);
		i -= 1;
		while (g_envp[++i] && g_envp[i + 1])
			g_envp[i] = free_str(g_envp[i], ft_strdup(g_envp[i + 1]));
		ft_strdel(&g_envp[i]);
	}
	return (1);
}
