#ifndef SHELL_H
# define SHELL_H

# include <signal.h>

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

/*
**	BASE
*/
typedef struct		s_builtin
{
	char			*command;
	_Bool			(*fct)(char **cmd);
}					t_builtin;

extern t_builtin	g_builtin[NBRBUILTIN];
extern char			**g_envp;

void				ft_run_shell(_Bool *ret, char *line);
void				sigint_handle(int sig);

/*
**	BUILTINS
*/
_Bool				ft_builtin_exit(char **ptr);
_Bool				ft_builtin_cd(char **ptr);
_Bool				ft_builtin_echo(char **ptr);
_Bool				ft_change_dir(char *str, int on);

_Bool				ft_builtin_env(char **ptr);
char				**ft_addenv(char *str);
_Bool				ft_builtin_setenv(char **ptr);
_Bool				ft_builtin_unsetenv(char **ptr);

char				**ft_newenv(char **envp);
int					ft_envar(char *name);
void				ft_setenv(char *name, char *value);

size_t				ft_ptrlen(char **ptr);
void				ft_ptrdel(char **ptr);
_Bool				ft_return_access(const char *name, char *str);

/*
**	lexer_parser.c
*/

# define WHITESPACE str[t->i] == ' ' || str[t->i] == '\t' || str[t->i] == '\n'
# define PREV_WHITESPACE t->prev == ' ' || t->prev == '\t' || t->prev == '\n'
# define QUOTE str[t->i] == '`' || str[t->i] == '\'' || str[t->i] == '\"'
# define SPECIAL_CHAR str[t->i] == '\\' || str[t->i] == '$' || str[t->i] == '~'
# define OPERATOR str[t->i] == '|' || str[t->i] == ';' || MORE_OP
# define MORE_OP str[t->i] == '<' || str[t->i] == '>' || str[t->i] == '&'
# define PREV_OPERATOR t->prev == '|' || t->prev == ';' || MORE_P_OP
# define MORE_P_OP t->prev == '<' || t->prev == '>' || t->prev == '&'

typedef struct		s_ast
{
	char			*val;
	int				expand;
	int				operator;
	struct s_ast	*parent;
	struct s_ast	*l_child;
	struct s_ast	*r_child;
}					t_ast;

typedef struct		s_info
{
	int				i;
	int				j;
	int				expand;
	int				operator;
	int				bs_index;
	int				number_bs;
	char			prev;
	t_ast			*tokens;
}					t_info;

t_ast				*parser(t_ast **tokens, t_ast *parent);
int					count_backslashes(int index, char *str);
void				add_token(t_info *t, int i, int j, char *str);
void				quoting(char *str, t_info *t);
void				tokenize(char *str, t_info *t, int len);
void				ft_restart(t_ast *tokens, char *msg);
void				check_quotes(char *str, t_info *t, t_ast **tokens);
void				free_ast(t_ast *tokens);

char				*ft_expand(char *str, _Bool tilde);
void				ft_check_expand(t_ast *tokens);

/*
**	ft_operator.c
*/
typedef struct		s_operator
{
	char			*value;
	void			(*fct)(t_ast *tokens, _Bool *ret);
}					t_operator;

# define NBRTOKENS 8

extern t_operator	g_operator[NBRTOKENS];

int					ft_run_cmd(char **cmd);

int					ft_len_cmd(t_ast *tokens);
_Bool				ft_tokens_exec(t_ast *tokens);
void				ft_tokens_semicolon(t_ast *tokens, _Bool *ret);
void				ft_tokens_pipe(t_ast *tokens, _Bool *ret);

void				ft_tokens_and(t_ast *tokens, _Bool *ret);
void				ft_tokens_or(t_ast *tokens, _Bool *ret);
void				ft_tokens_redirect(t_ast *tokens, _Bool *ret);
void				ft_tokens_redirect_append(t_ast *tokens, _Bool *ret);
void				ft_tokens_redirect_fd(t_ast *tokens, _Bool *ret);

void				ft_tokens_here_doc(t_ast *tokens, _Bool *ret);
int					ft_check_operator(char *str);
char				*ft_tokens_val(t_ast *tokens);

/*
** HISTORY
*/
typedef struct		s_hist
{
	int				index;
	char			*command;
	struct s_hist	*next;
}					t_history;

extern t_history	*g_history;
extern char			*g_history_file;
# define STDOUTPUT 1

void				ft_history_add(char *command);
void				ft_print_history(t_history *history, int index, int io);
void				ft_write_history(void);
void				ft_free_history(void);
char				**ft_checkhistory(char **cmd);

int					ft_numlen(char *str);
_Bool				ft_builtin_history(char **ptr);
char				*get_history_file(void);

char				*ft_check_history(char *str);

void				ft_dup_fd(int fd_origin, int fd_new, int io);
char				*get_backquote(char *str, char c);

#endif
