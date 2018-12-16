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
	int		i;
	char	*variable;

	i = -1;
	while (g_envp[++i])
	{
		variable = ft_strmcpy(g_envp[i], ENVNAME(g_envp[i]));
		if (ft_strcmp(variable, name) == 0)
			break ;
		ft_strdel(&variable);
	}
	ft_strdel(&variable);
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
