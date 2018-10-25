/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjacques <mjacques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/15 14:15:18 by mjacques          #+#    #+#             */
/*   Updated: 2018/10/24 20:54:55 by mjacques         ###   ########.fr       */
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
		// if ((command = ft_init()) && *command)
		// 	ret = ft_run_cmd(command);
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

_Bool	execute_tokens(t_ast *tokens)
{
	int		i;
	int		stat_loc;
	_Bool	ret;
	char	**command;
	pid_t	pid;
	pid_t	pid1;

	command = NULL;
	ret = 1;
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
				if((pid = fork()) == 0)
				{
					if((pid1 = fork()) != 0)
					{
						waitpid(pid1, &stat_loc, WUNTRACED);
						(fd[1] != 0) ? close(fd[1]) : 0;
						dup2(fd[0], 0);
						(fd[0] != 0) ? close(fd[0]) : 0;
						//execlp("cat", "cat", "-e", NULL);
						ret = execute_tokens(tokens->r_child);
					}
					else
					{
						(fd[0] != 0) ? close(fd[0]) : 0;
						dup2(fd[1], 1);
						(fd[1] != 0) ? close(fd[1]) : 0;
						//		execve("/bin/ls", command, NULL);
						ret = execute_tokens(tokens->l_child);
					}
				}
				(fd[0] != 0) ? close(fd[0]) : 0;
				(fd[1] != 0) ? close(fd[1]) : 0;
				waitpid(pid, &stat_loc, WUNTRACED);
			}
			else if (ft_strcmp(tokens->val, ">") == 0)
			{
				if ((pid = fork()) == 0)
				{
					fd[1] = open(tokens->r_child->val, O_RDWR | O_CREAT | O_TRUNC, 0666);
					dup2(fd[1], 1);
					ret = execute_tokens(tokens->l_child);
					close(fd[1]);
				}
				(fd[1] != 0) ? close(fd[1]) : 0;
				waitpid(pid, &stat_loc, WUNTRACED);
			}
			else if (ft_strcmp(tokens->val, ">>") == 0)
			{
				if ((pid = fork()) == 0)
				{
					fd[1] = open(tokens->r_child->val, O_RDWR | O_CREAT, 0666);
					dup2(fd[1], 1);
					ret = execute_tokens(tokens->l_child);
					close(fd[1]);
				}
				(fd[1] != 0) ? close(fd[1]) : 0;
				waitpid(pid, &stat_loc, WUNTRACED);
			}
			else
				ft_putendl("Wait Wait!");
		}
		else
		{
			command = (char **)ft_memalloc(sizeof(char *) * (ft_len_cmd(tokens) + 1));
			i = -1;
			while (tokens)
			{
				command[++i] = ft_strdup(tokens->val);
				tokens = tokens->l_child;
			}
			command = ft_checkquote(command);
			command = ft_checkenv(command);
			ret = ft_run_cmd(command);
		}
	}
	return (ret);
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
	return (ret);
}
