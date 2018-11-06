/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjacques <mjacques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/15 17:42:49 by mjacques          #+#    #+#             */
/*   Updated: 2018/11/05 18:11:14 by mcarney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

_Bool			ft_builtin_exit(char **ptr)
{
	ft_ptrdel(g_envp);
	g_envp = NULL;
	ft_ptrdel(ptr);
	ptr = NULL;
	ft_printf("%s%sHave a nice day!%s\n", BOLD, YELLOW, REGULAR);
	ft_write_history();
	ft_free_history();
	exit(0);
	return (0);
}

_Bool			ft_builtin_echo(char **ptr)
{
	int	i;

	if (ptr[1])
	{
		i = (ft_strcmp(ptr[1], "-n")) ? 0 : 1;
		while (ptr[++i])
		{
			ft_putstr(ptr[i]);
			(ptr[i + 1]) ? ft_putchar(' ') : NULL;
		}
		(!ft_strcmp(ptr[1], "-n")) ? NULL : ft_putchar('\n');
	}
	else
		ft_putendl("");
	return (0);
}

_Bool			ft_builtin_cd(char **ptr)
{
	int		j;
	int		ret;
	int		on;
	char	*str;

	on = 0;
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
		if (!ft_strcmp(ptr[1], "-") && (on = 1))
			str = (j != -1) ? ft_strdup(&g_envp[j][7]) : NULL;
		else
			str = ft_strdup(ptr[1]);
		ret = ft_change_dir(str, on);
	}
	else
		ft_putendl("Usage: cd [directory]");
	return (ret);
}

static _Bool	ft_change_pwd(void)
{
	int		j;
	char	*pwd;
	char	*tmp;
	char	cwd[PATH_MAX + 1];

	j = ft_envar("PWD");
	pwd = (j != -1) ? ft_strdup(&g_envp[j][4]) : NULL;
	tmp = ft_strdup(getcwd(cwd, PATH_MAX));
	ft_setenv("OLDPWD", pwd);
	ft_strdel(&pwd);
	ft_setenv("PWD", tmp);
	ft_strdel(&tmp);
	return (0);
}

_Bool			ft_change_dir(char *str, int on)
{
	int	ret;

	ret = 1;
	if (!chdir(str))
	{
		(on) ? ft_putendl(str) : NULL;
		ret = ft_change_pwd();
	}
	else
		(!str) ? ft_putchar('\n') : ft_return_access("cd", str);
	ft_strdel(&str);
	return (ret);
}
