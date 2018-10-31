/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_envp_tools.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjacques <mjacques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/15 17:40:01 by mjacques          #+#    #+#             */
/*   Updated: 2018/10/30 23:17:18 by mjacques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

char		**ft_newenv(char **envp)
{
	int		size;

	size = ft_ptrlen(envp);
	g_envp = (char **)malloc(sizeof(char *) * (size + 1));
	size = -1;
	while (envp[++size])
		g_envp[size] = ft_strdup(envp[size]);
	g_envp[size] = NULL;
	return (g_envp);
}

int			ft_envar(char *name)
{
	int i;

	i = -1;
	while (g_envp[++i])
		if (!ft_strncmp(g_envp[i], name, ft_strlen(name)))
			break ;
	i = (g_envp[i]) ? i : -1;
	return (i);
}

void		ft_setenv(char *name, char *value)
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
}
