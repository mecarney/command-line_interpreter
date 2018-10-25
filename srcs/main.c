/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjacques <mjacques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/15 14:15:18 by mjacques          #+#    #+#             */
/*   Updated: 2018/10/24 23:39:22 by fhong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int			fd[2];
char		**g_envp;
t_builtin	g_builtin[NBRBUILTIN] = {
	{"cd", &ft_builtin_cd},
	{"echo", &ft_builtin_echo},
	{"setenv", &ft_builtin_setenv},
	{"unsetenv", &ft_builtin_unsetenv},
	{"env", &ft_builtin_env},
	{"exit", &ft_builtin_exit}
};

int		main(int argc, char **argv, char **envp)
{
	_Bool	ret;
	char	**command;

	(argc != 1 || !argv[0]) ? ft_error("Usage: ./minishell") : NULL;
	ft_printf("%s%sWelcome!%s Make great code today\n", BOLD, BLUE, REGULAR);
	envp = ft_newenv(envp);
	ret = 0;
	while (!(command = NULL))
	{
		(ret) ? ft_putstr(RED) : ft_putstr(PURPLE);
		ret = ft_init();
		ft_ptrdel(command);
	}
	return (0);
}

int		ft_len_cmd(t_ast *tokens)
{
	int		len;

	len = 0;
	while (tokens)
	{
		len++;
		tokens = tokens->l_child;
	}
	return (len);
}

void	ft_tokens_to_cmd(t_ast *tokens, _Bool *ret)
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
	command = ft_checkquote(command);
	command = ft_checkenv(command);
	*ret = ft_run_cmd(command);
	ft_ptrdel(command);
}

_Bool	execute_tokens(t_ast *tokens)
{
	int		fd_origin0;
	int		fd_origin1;
	int		stat_loc;
	pid_t	pid;
	_Bool	ret;

	ret = 1;
	fd_origin0 = dup(0);
	fd_origin1 = dup(1);
	if (tokens)
	{
		if (tokens->r_child)
		{
			if (ft_strcmp(tokens->val, ";") == 0)
			{
				ret = execute_tokens(tokens->l_child);
				ret = execute_tokens(tokens->r_child);
			}
			else if (ft_strcmp(tokens->val, "&&") == 0)
			{
				if ((ret = execute_tokens(tokens->l_child)) == 0)
					ret = execute_tokens(tokens->r_child);
			}
			else if (ft_strcmp(tokens->val, "||") == 0)
			{
				if ((ret = execute_tokens(tokens->l_child)) == 1)
					ret = execute_tokens(tokens->r_child);
			}
			else if (ft_strcmp(tokens->val, "|") == 0)
			{
				pipe(fd);
				if((pid = fork()) != 0)
				{
					waitpid(pid, &stat_loc, WUNTRACED);
					dup2(fd_origin1, 1);
					close(fd[1]);
					dup2(fd[0], 0);
					close(fd[0]);
					ret = execute_tokens(tokens->r_child);
					dup2(fd_origin0, 0);
				}
				else if (pid == 0)
				{
					close(fd[0]);
					dup2(fd[1], 1);
					close(fd[1]);
					ret = execute_tokens(tokens->l_child);
					exit(1);
				}
			}
			else if (ft_strcmp(tokens->val, ">") == 0)
			{
				if ((fd[1] = open(tokens->r_child->val, O_RDWR | O_CREAT | O_TRUNC, 0666)) == -1)
				{
					ft_printf("%s: Permission denied\n", tokens->r_child->val);
					return (1);
				}
				dup2(fd[1], 1);
				ret = execute_tokens(tokens->l_child);
				close(fd[1]);
				dup2(fd_origin1, 1);
			}
			else if (ft_strcmp(tokens->val, ">>") == 0)
			{
				if ((fd[1] = open(tokens->r_child->val, O_RDWR | O_CREAT, 0666)) == -1)
				{
					ft_printf("%s: Permission denied\n", tokens->r_child->val);
					return (1);
				}
				dup2(fd[1], 1);
				lseek(fd[1], 0, SEEK_END);
				ret = execute_tokens(tokens->l_child);
				close(fd[1]);
				dup2(fd_origin1, 1);
			}
			else
				ft_putendl("Attend, Wait!");
		}
		else
			ft_tokens_to_cmd(tokens, &ret);
	}
	return (ret);
}

void	free_ast(t_ast *tokens)
{
	t_ast	*tmp;

	if (!(tokens))
		return ;
	if (!(tokens->r_child))
	{
		while (tokens->l_child)
		{
			tmp = tokens;
			tokens = tokens->l_child;
			ft_strdel(&tmp->val);
			free(tmp);
		}
	}
	else
	{
		if (tokens->l_child)
			free_ast(tokens->l_child);
		if (tokens->r_child)
			free_ast(tokens->r_child);
	}
	ft_strdel(&tokens->val);
	free(tokens);
}

_Bool	ft_init(void)
{
	char	*line;
	_Bool	ret;
	t_okenize				t;
	t_ast			*tokens;


	ft_putstr(BOLD);
	ft_putstr(SHELLNAME);
	ft_putstr(REGULAR);
	if (get_next_line(0, &line) <= 0)
		return (NULL);
	defaults(&t);
	tokens = NULL;
	check_quotes(line, &t, &tokens);
	// printf("%d\n", t.tokens);
	tokens = parser(&tokens, NULL);
	// tokens ? printf("%s \n", tokens->val) : 0;
	// print_ast(tokens);
	ret = execute_tokens(tokens);
	ft_strdel(&line);
	free_ast(tokens);
	return (ret);
}
