/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjacques <mjacques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/15 14:27:48 by mjacques          #+#    #+#             */
/*   Updated: 2018/10/27 16:30:05 by mjacques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include "libft.h"

# define SHELLNAME	"\033[1m42sh% \033[0m"
# define NBRBUILTIN 7
# define ENVNAME(x) (ft_strchr(x, '=') - x)
# define PATH_MAX 4096

# define REGULAR "\033[0m"
# define BOLD "\033[1m"
# define RED "\033[31m"
# define BLUE "\033[92m"
# define PURPLE "\033[34m"
# define YELLOW "\033[33m"
# define STDOUTPUT 1 // for normal print history or write the command in .42sh_history

typedef struct		s_hist
{
	int				index;
	char			*command;
	struct s_hist 	*next;
}					t_history;

typedef struct		s_builtin
{
	char			*command;
	_Bool			(*fct)(char **cmd);
}					t_builtin;

extern t_builtin	g_builtin[NBRBUILTIN];
extern char			**g_envp;
extern t_history	*g_history;
extern char			*g_history_file;

/*
**	main.c
*/
void				ft_init(_Bool *ret);

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
char				**ft_addenv(char *str);
_Bool				ft_builtin_setenv(char **ptr);
_Bool				ft_builtin_unsetenv(char **ptr);

/*
**	ft_envptools.c
*/
char				**ft_newenv(char **envp);
char				**ft_checkenv(char **cmd);
char				**ft_checkquote(char **cmd);
char				**ft_checkhistory(char **cmd);
int					ft_envar(char *name);
void				ft_setenv(char *name, char *value);

/*
**	ft_command.c
*/
int					ft_run_cmd(char **cmd);

/*
**	ft_helper.c
*/
size_t				ft_ptrlen(char **ptr);
void				ft_ptrdel(char **ptr);
_Bool				ft_change_dir(char *str, int on);
_Bool				ft_return_access(const char *name, char *str);

/*
**	lexer_parser.c
*/

typedef struct		s_okenize
{
	int				i;
	int				j;
	int				tokens;
	char			prev;
}					t_okenize;

typedef struct		s_ast
{
	char			*val;
	struct s_ast	*parent;
	struct s_ast	*l_child;
	struct s_ast	*r_child;
}					t_ast;

t_ast				*search(t_ast **tokens, int *n, char *str, size_t len);
t_ast				*parser(t_ast **tokens, t_ast *parent);
void				add_token(t_okenize *t, int i, int j, t_ast **tokens, char *str);
void				quoting(char *str, t_okenize *t, t_ast **tokens);
int					is_operator(char a);
void				tokenize(char *str, t_okenize *t, t_ast **tokens);
void				defaults(t_okenize *t);
void				print_ast(t_ast *tokens);
void				append_str(char *str, t_okenize *t, t_ast **tokens, char *msg);
int					check_operator(char *str, t_okenize *t, t_ast **tokens);
void				check_quotes(char *str, t_okenize *t, t_ast **tokens);
int					ft_strfind(const char *s1, const char *s2);
void				free_ast(t_ast *tokens);

/*
**	ft_operator.c
*/
typedef struct		s_operator
{
	char			*value;
	void			(*fct)(t_ast *tokens, _Bool *ret);
}					t_operator;

# define NBRTOKENS 7
extern t_operator	g_operator[NBRTOKENS];

int					ft_len_cmd(t_ast *tokens);
void				ft_tokens_to_cmd(t_ast *tokens, _Bool *ret);
_Bool				ft_tokens_exec(t_ast *tokens);
void				ft_tokens_semicolon(t_ast *tokens, _Bool *ret);
void				ft_tokens_pipe(t_ast *tokens, _Bool *ret);

void				ft_tokens_and(t_ast *tokens, _Bool *ret);
void				ft_tokens_or(t_ast *tokens, _Bool *ret);
void				ft_tokens_redirect(t_ast *tokens, _Bool *ret);
void				ft_tokens_redirect_append(t_ast *tokens, _Bool *ret);
void				ft_tokens_redirect_fd(t_ast *tokens, _Bool *ret);

int					ft_check_operator(char *str);
char				*ft_tokens_val(t_ast *tokens);


/*
** ft_historytools.c
*/

void				ft_history_add(char *command);
void				ft_print_history(t_history *history, int index, int io);
void				ft_write_history(void);
void				ft_free_history(void);

/*
** ft_history.c
*/

int					ft_numlen(char *str);
_Bool				ft_builtin_history(char **ptr);
char				*get_history_file(void);

/*
** ft_check_history
*/

char				*ft_check_history(char *str);

#endif
