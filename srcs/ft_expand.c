/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjacques <mjacques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/30 23:17:33 by mjacques          #+#    #+#             */
/*   Updated: 2018/11/09 15:27:45 by mcarney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static char	*ft_variable_expand(char *str, char *name, int pos)
{
	int		envar;
	int		len;
	char	*tmp;

	envar = ft_envar(name);
	len = ft_strlen(name) + 1;
	if (envar == -1)
		tmp = ft_strdup(&str[pos]);
	else
		tmp = ft_strjoin(&g_envp[envar][len], &str[pos]);
	return (tmp);
}

static char	*ft_check_tilde(char *cmd)
{
	if (!cmd[1])
		cmd = free_str(cmd, ft_variable_expand(cmd, "HOME", 1));
	else if (cmd[1] && (cmd[1] == '/'))
		cmd = free_str(cmd, ft_variable_expand(cmd, "HOME", 1));
	return (cmd);
}

static char	*ft_variable_len(char *cmd, int *pos)
{
	int		len;
	char	*name;

	len = 0;
	while (cmd[*pos + len] && cmd[*pos + len] > 47)
		len++;
	name = ft_strmcpy(&cmd[*pos], len);
	*pos += len - 1;
	return (name);
}

static char	*ft_check_dollar(char *cmd, int *dollar)
{
	int		pos;
	char	*back;
	char	*name;
	char	*expand;

	pos = *dollar;
	back = ft_strsub(cmd, 0, pos);
	pos += 1;
	name = ft_variable_len(cmd, &pos);
	if (name && *name)
	{
		expand = ft_variable_expand(cmd, name, pos + 1);
		cmd = free_str(cmd, ft_strjoin(back, expand));
		ft_strdel(&expand);
	}
	else
		*dollar += 1;
	ft_strdel(&name);
	ft_strdel(&back);
	return (cmd);
}

char		*ft_expand(char *str, _Bool tilde)
{
	int		pos;
	char	*position;

	pos = 0;
	if (tilde && str[0] && str[0] == '~')
		str = ft_check_tilde(str);
	while ((position = ft_strchr(&str[pos], '$')))
	{
		pos = (position - str);
		str = ft_check_dollar(str, &pos);
	}
	str = ft_check_history(str);
	return (str);
}
