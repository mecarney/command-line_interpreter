/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjacques <mjacques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/15 14:27:48 by mjacques          #+#    #+#             */
/*   Updated: 2018/10/23 19:10:08 by mjacques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include <dirent.h>

# include "libft.h"

# define SHELLNAME	"\033[1m\033[34m42sh%\033[0m "
# define NBRBUILTIN 6
# define ENVNAME(x) (ft_strchr(x, '=') - x)
# define PATH_MAX 4096

typedef struct		s_builtin
{
	char			*command;
	_Bool			(*fct)(char **cmd);
}					t_builtin;

extern t_builtin	g_builtin[NBRBUILTIN];
extern char			**g_envp;

/*
**	main.c
*/
char				**ft_init(void);

/*
**	ft_builtins.c
*/
_Bool				ft_builtin(char **ptr);
_Bool				ft_builtin_exit(char **ptr);
_Bool				ft_builtin_cd(char **ptr);
_Bool				ft_builtin_echo(char **ptr);

/*
**	ft_environement.c
*/
_Bool				ft_builtin_env(char **ptr);
_Bool				ft_setenv(char *name, char *value);
_Bool				ft_builtin_setenv(char **ptr);
_Bool				ft_builtin_unsetenv(char **ptr);

/*
**	ft_tools.c
*/
char				**ft_newenv(char **envp);
char				**ft_checkenv(char **cmd);
char				**ft_checkquote(char **cmd);
int					ft_envar(char *name);
char				**ft_addenv(char *str);

/*
**	ft_command.c
*/
void				ft_run_system_cmd(char **ptr);
int					ft_check_builtin(char **cmd);
void				ft_run_cmd(char **cmd);

/*
**	ft_helper.c
*/
size_t				ft_ptrlen(char **ptr);
void				ft_ptrdel(char **ptr);
void				ft_change_dir(char *str, int on);

#endif
