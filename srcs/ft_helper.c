/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_helper.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjacques <mjacques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 19:39:02 by mjacques          #+#    #+#             */
/*   Updated: 2018/10/23 17:11:12 by mjacques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

size_t		ft_ptrlen(char **ptr)
{
	size_t	size;

	size = 0;
	while (ptr[size])
		size++;
	return (size);
}

void		ft_ptrdel(char **ptr)
{
	int i;

	if (!ptr)
		return ;
	i = -1;
	while (ptr[++i])
		ft_strdel(&ptr[i]);
	free(ptr);
}

static void	ft_change_pwd(void)
{
	int		j;
	char	*pwd;
	char	*tmp;
	char	cwd[PATH_MAX + 1];

	j = ft_envar("PWD");
	pwd = (j != -1) ? ft_strdup(&g_envp[j][4]) : NULL;
	tmp = ft_strdup(getcwd(cwd, PATH_MAX));
	ft_setenv("OLDPWD", pwd);
	ft_setenv("PWD", tmp);
	ft_strdel(&pwd);
	ft_strdel(&tmp);
}

void		ft_change_dir(char *str, int on)
{
	if (!chdir(str))
	{
		(on) ? ft_putendl(str) : NULL;
		ft_change_pwd();
	}
	else
	{
		if (access(str, F_OK) == -1)
			ft_printf("cd: no such file or directory: %s\n", str);
		else if (access(str, R_OK) == -1)
			ft_printf("cd: permission denied: %s\n", str);
		else
			ft_printf("cd: not a directory: %s\n", str);
	}
	ft_strdel(&str);
}
