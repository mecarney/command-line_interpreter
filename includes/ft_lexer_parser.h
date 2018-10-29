/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer_parser.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjacques <mjacques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/28 23:46:13 by mjacques          #+#    #+#             */
/*   Updated: 2018/10/28 23:47:18 by mjacques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LEXER_PARSER
#define FT_LEXER_PARSER

# include "shell.h"

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

#endif
