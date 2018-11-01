/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjacques <mjacques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/30 23:17:33 by mjacques          #+#    #+#             */
/*   Updated: 2018/11/01 15:43:03 by mjacques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static char	*ft_expand(char *str, char *name, int pos)
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
		cmd = free_str(cmd, ft_expand(cmd, "HOME", 1));
	else if (cmd[1] && (cmd[1] == '/'))
		cmd = free_str(cmd, ft_expand(cmd, "HOME", 1));
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
		expand = ft_expand(cmd, name, pos + 1);
		cmd = free_str(cmd, ft_strjoin(back, expand));
		ft_strdel(&expand);
	}
	else
		*dollar += 1;
	ft_strdel(&name);
	ft_strdel(&back);
	return (cmd);
}

void		ft_check_expand(t_ast *tokens)
{
	int		pos;
	t_ast	*tmp;
	char	*position;

	tmp = tokens;
	while (tmp)
	{
		if (tmp->expand)
		{
			pos = 0;
			if (tmp->val[0] && tmp->val[0] == '~')
				tmp->val = ft_check_tilde(tmp->val);
			while ((position = ft_strchr(&tmp->val[pos], '$')))
			{
				pos = (position - tmp->val);
				tmp->val = ft_check_dollar(tmp->val, &pos);
			}
		}
		tmp = tmp->l_child;
	}
}
