#include "shell.h"

int			ft_check_operator(char *str)
{
	int i;

	i = -1;
	while (++i < NBRTOKENS)
		if (ft_strcmp(str, g_operator[i].value) == 0)
			return (i);
	return (-1);
}

char		*ft_tokens_val(t_ast *tokens)
{
	if (ft_check_operator(tokens->val) == -1)
		return (tokens->val);
	return (ft_tokens_val(tokens->l_child));
}

_Bool		ft_tokens_file(int fd)
{
	char	*line;

	line = NULL;
	while (get_next_line(fd, &line) > 0)
	{
		ft_putendl(line);
		ft_strdel(&line);
	}
	return (0);
}
