/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_helper.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjacques <mjacques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 19:39:02 by mjacques          #+#    #+#             */
/*   Updated: 2018/10/25 12:07:17 by mjacques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

size_t			ft_ptrlen(char **ptr)
{
	size_t	size;

	size = 0;
	while (ptr[size])
		size++;
	return (size);
}

void			ft_ptrdel(char **ptr)
{
	int i;

	if (!ptr)
		return ;
	i = -1;
	while (ptr[++i])
		ft_strdel(&ptr[i]);
	free(ptr);
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
		ret = ft_return_access("cd", str);
	ft_strdel(&str);
	return (ret);
}

_Bool			ft_return_access(const char *name, char *str)
{
	if (access(str, F_OK) == -1)
		ft_printf("%s: no such file or directory: %s\n", name, str);
	else if (access(str, R_OK) == -1)
		ft_printf("%s: permission denied: %s\n", name, str);
	else
	{
		if (ft_strcmp(name, "cd") == 0)
			ft_printf("%s: not a directory: %s\n", name, str);
		else
			ft_printf("%s: is directory: %s\n", name, str);
	}
	return (1);
}
