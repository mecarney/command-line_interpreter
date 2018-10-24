/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_envptools.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjacques <mjacques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/15 17:40:01 by mjacques          #+#    #+#             */
/*   Updated: 2018/10/23 17:11:15 by mjacques         ###   ########.fr       */
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

char		**ft_checkenv(char **cmd)
{
	int		i;
	int		j;
	int		len;

	i = -1;
	while (cmd[++i])
	{
		len = ft_strlen(cmd[i]);
		if (cmd[i][0] == '$' && (j = -1) == -1)
			while (g_envp[++j])
				if (!ft_strncmp(g_envp[j], &cmd[i][1], ENVNAME(g_envp[j])))
					cmd[i] = free_str(cmd[i], ft_strdup(&g_envp[j][len]));
		if (!ft_strcmp(cmd[i], "~") && (j = -1) == -1)
			while (g_envp[++j])
				if (ft_strncmp(g_envp[j], "HOME", 4) == 0)
					cmd[i] = free_str(cmd[i], ft_strdup(&g_envp[j][5]));
	}
	return (cmd);
}

char		**ft_checkquote(char **cmd)
{
	int		i;
	int		j;
	int		k;
	char	*str;

	i = -1;
	while (cmd[++i])
	{
		str = ft_strnew(ft_strlen(cmd[i]));
		j = -1;
		k = -1;
		while (cmd[i][++j])
			if (cmd[i][j] != '\"')
				str[++k] = cmd[i][j];
		cmd[i] = free_str(cmd[i], ft_strdup(str));
		ft_strdel(&str);
	}
	return (cmd);
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

char		**ft_addenv(char *str)
{
	int		size;
	char	**newenv;

	size = ft_ptrlen(g_envp);
	newenv = (char **)ft_memalloc(sizeof(char *) * (size + 2));
	size = -1;
	while (g_envp[++size])
		newenv[size] = ft_strdup(g_envp[size]);
	newenv[size] = ft_strdup(str);
	newenv[size + 1] = NULL;
	ft_ptrdel(g_envp);
	return (newenv);
}
