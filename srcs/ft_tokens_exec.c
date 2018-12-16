#include "shell.h"

t_operator	g_operator[NBRTOKENS] = {
	{";", &ft_tokens_semicolon},
	{"&&", &ft_tokens_and},
	{"||", &ft_tokens_or},
	{"|", &ft_tokens_pipe},
	{">", &ft_tokens_redirect},
	{">>", &ft_tokens_redirect_append},
	{"<", &ft_tokens_redirect_fd},
	{"<<", &ft_tokens_here_doc}
};

void		ft_tokens_to_cmd(t_ast *tokens, _Bool *ret)
{
	int		i;
	int		len;
	char	**command;

	len = ft_len_cmd(tokens);
	command = (char **)ft_memalloc(sizeof(char *) * (len + 1));
	i = -1;
	while (tokens)
	{
		command[++i] = ft_strdup(tokens->val);
		tokens = tokens->l_child;
	}
	*ret = ft_run_cmd(command);
	ft_ptrdel(command);
}

_Bool		ft_tokens_exec(t_ast *tokens)
{
	int		i;
	_Bool	ret;

	ret = 1;
	if (tokens)
	{
		if (tokens->r_child)
		{
			if ((i = ft_check_operator(tokens->val)) != -1)
				g_operator[i].fct(tokens, &ret);
			return (ret);
		}
		if (ft_strcmp(tokens->val, ";") == 0 && tokens->l_child)
			tokens = tokens->l_child;
		ft_check_expand(tokens);
		ft_tokens_to_cmd(tokens, &ret);
	}
	return (ret);
}

void		ft_tokens_semicolon(t_ast *tokens, _Bool *ret)
{
	*ret = ft_tokens_exec(tokens->l_child);
	*ret = ft_tokens_exec(tokens->r_child);
}

void		ft_dup_fd(int fd_origin, int fd_new, int io)
{
	close(fd_origin);
	dup2(fd_new, io);
	close(fd_new);
}

void		ft_tokens_pipe(t_ast *tokens, _Bool *ret)
{
	int		stat_loc;
	int		fd[2];
	pid_t	child_left;
	pid_t	child_right;

	pipe(fd);
	if ((child_left = fork()) == 0)
	{
		ft_dup_fd(fd[0], fd[1], 1);
		exit(ft_tokens_exec(tokens->l_child));
	}
	else if (child_left < 0)
		ft_putendl("ERROR: fork() failed");
	if ((child_right = fork()) == 0)
	{
		ft_dup_fd(fd[1], fd[0], 0);
		exit(ft_tokens_exec(tokens->r_child));
	}
	else if (child_right < 0)
		ft_putendl("ERROR: fork() failed");
	close(fd[0]);
	close(fd[1]);
	waitpid(child_left, 0, WUNTRACED);
	waitpid(child_right, &stat_loc, WUNTRACED);
	*ret = stat_loc;
}
