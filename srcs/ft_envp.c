#include "shell.h"

static int	ft_variable(char **ptr)
{
	int	i;

	i = 0;
	while (ptr[++i])
		if (!ft_strchr(ptr[i], '='))
			return (i);
	return (0);
}

_Bool		ft_builtin_env(char **ptr)
{
	int i;
	int j;
	int	on;

	if ((j = ft_variable(ptr)))
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
	return (0);
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

_Bool		ft_builtin_setenv(char **ptr)
{
	int		i;
	int		ret;
	char	*name;

	ret = 1;
	if (ptr[1] && !ptr[2] && !ft_variable(ptr))
	{
		name = ft_strmcpy(ptr[1], ENVNAME(ptr[1]));
		i = ft_envar(name);
		if (i != -1)
			g_envp[i] = free_str(g_envp[i], ft_strdup(ptr[1]));
		else
			g_envp = ft_addenv(ptr[1]);
		ft_strdel(&name);
		ret = 0;
	}
	(ret) ? ft_putendl("Usage: setenv name=[value]") : NULL;
	return (ret);
}

_Bool		ft_builtin_unsetenv(char **ptr)
{
	int	i;
	int	ret;

	ret = 1;
	if (ptr[1] && !ptr[2] && ft_variable(ptr) && (i = ft_envar(ptr[1])) != -1)
	{
		i -= 1;
		while (g_envp[++i] && g_envp[i + 1])
			g_envp[i] = free_str(g_envp[i], ft_strdup(g_envp[i + 1]));
		ft_strdel(&g_envp[i]);
		ret = 0;
	}
	(ret) ? ft_putendl("Usage: unsetenv name") : NULL;
	return (ret);
}
