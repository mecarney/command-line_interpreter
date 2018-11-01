/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjacques <mjacques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/30 23:17:33 by mjacques          #+#    #+#             */
/*   Updated: 2018/11/01 14:23:27 by mjacques         ###   ########.fr       */
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

static char	*ft_check_dollar(char *cmd, char *dollar)
{
	int		pos;
	char	*name;
	char	*expand;

	pos = dollar - cmd;
	dollar = ft_strsub(cmd, 0, pos);
	pos += 1;
	name = ft_variable_len(cmd, &pos);
	expand = ft_expand(cmd, name, pos + 1);
	ft_strdel(&name);
	cmd = free_str(cmd, ft_strjoin(dollar, expand));
	ft_strdel(&dollar);
	ft_strdel(&expand);
	return (cmd);
}

void		ft_check_expand(t_ast *tokens)
{
	t_ast	*tmp;
	char	*position;

	tmp = tokens;
	while (tmp)
	{
		ft_printf("Ex: %d | Str: %s\n", tmp->expand, tmp->val);
		if (tmp->expand)
		{
			if (tmp->val[0] && tmp->val[0] == '~')
				tmp->val = ft_check_tilde(tmp->val);
			while ((position = ft_strchr(tmp->val, '$')))
				tmp->val = ft_check_dollar(tmp->val, position);
		}
		tmp = tmp->l_child;
	}
}
