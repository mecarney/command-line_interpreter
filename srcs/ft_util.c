#include "shell.h"

int		ft_len_cmd(t_ast *tokens)
{
	int				len;

	len = 0;
	while (tokens)
	{
		len++;
		tokens = tokens->l_child;
	}
	return (len);
}

void	ft_check_expand(t_ast *tokens)
{
	t_ast	*tmp;

	tmp = tokens;
	while (tmp)
	{
		if ((*tmp->val) && tmp->expand == 2)
			tmp->val = get_backquote(tmp->val, ' ');
		else if ((*tmp->val) && tmp->expand == 1)
			tmp->val = ft_expand(tmp->val, 1);
		if (!(tmp->val))
			ft_restart(tokens, "Null expansion");
		tmp = tmp->l_child;
	}
}

void	sigint_handle(int sig)
{
	(void)sig;
	ft_putendl(RED);
	ft_putstr(SHELLNAME);
}
