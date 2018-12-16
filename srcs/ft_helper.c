#include "shell.h"

size_t	ft_ptrlen(char **ptr)
{
	size_t	size;

	size = 0;
	while (ptr[size])
		size++;
	return (size);
}

void	ft_ptrdel(char **ptr)
{
	int i;

	if (!ptr)
		return ;
	i = -1;
	while (ptr[++i])
		ft_strdel(&ptr[i]);
	free(ptr);
}

_Bool	ft_return_access(const char *name, char *str)
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
