/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenizer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcarney <mcarney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/26 15:55:08 by mcarney           #+#    #+#             */
/*   Updated: 2018/11/01 14:29:30 by mjacques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void				add_token(t_okenize *t, int i, int j, t_ast **tokens, char *str, int expand)
{
	t_ast			*new;
	t_ast			*old;

	old = *tokens;
	if (!(new = (t_ast *)malloc(sizeof(t_ast))))
		ft_error("Malloc error");
	new->val = ft_strsub(str, j, i - j + 1);
	new->l_child = NULL;
	new->r_child = NULL;
	if (!(*tokens))
		*tokens = new;
	else
	{
		while (old->l_child)
			old = old->l_child;
		old->l_child = new;
	}
	t->j = t->i;
	t->tokens++;
	t->prev = '\0';
	new->expand = expand;
}

void				quoting(char *str, t_okenize *t, t_ast **tokens)
{
	char	ch;
	int		expand;

	expand = 0;
	ch = (str[t->i] != '(') ? str[t->i] : ')';
	if (ch == '\\')
	{
		if (!(t->prev))
			t->j = t->i;
		while (str[t->i] && str[t->i] !=  ' ' && str[t->i] != '\t' && str[t->i] != '\n')
			t->i++;
	}
	else if (ch == '$' || ch == '~')
	{
		if (!(t->prev))
			t->j = t->i;
		if (str[t->i + 1] && str[t->i] == '(')
		{
			while (str[t->i] && (str[t->i] != ')' ||\
					(str[t->i] == ')' && str[t->i - 1] == '\\')))
				t->i++;
		}
		else
		{
			while (str[t->i] && str[t->i] !=  ' ' && str[t->i] != '\t' && str[t->i] != '\n'  && str[t->i] != '\"'  && str[t->i] != '`'  && str[t->i] != '\'')
				t->i++;
		}
	}
	else
	{
		if (t->prev && t->prev != ' ' && t->prev != '\t' && t->prev != '\n')
			add_token(t, t->i - 1, t->j, tokens, str, 0);
		t->j = t->i++;
		while (str[t->i] && (str[t->i] != ch ||\
				(str[t->i] == ch && str[t->i - 1] == '\\')))
		{
			// (ch == '"' && str[t->i] == '$') ? expand = 1 : 0;
			t->i++;
		}
	}
	if (ch == '`')
	{
		add_token(t, ft_strlen(get_backquote(ft_strsub(str, t->j + 1, t->i - 1))), 0, tokens, get_backquote(ft_strsub(str, t->j + 1, t->i - 1)), 0);
	}
	else if (ch == '\'' || ch == '\"')
		add_token(t, t->i - 1, t->j + 1, tokens, str, expand);
	else
	{
		expand = (ch == '$' || ch == '~') ?  1 : 0;
		add_token(t, t->i - 1, t->j, tokens, str, expand);
	}
}

int					is_operator(char a)
{
	if (a == '|' || a == '&' || a == ';' || a == '<' || a == '>')
		return (1);
	return (0);
}

void 				tokenize(char *str, t_okenize *t, t_ast **tokens)
{
	int			len;

	len = ft_strlen(str);
	while (str && len > ++t->i && str[t->i] != '#')
	{
		if (t->prev && is_operator(t->prev) && str[t->i] == t->prev)
			add_token(t, t->i, t->i - 1, tokens, str, 0);
		else if (t->prev && is_operator(t->prev))// && is_operator(str[t->i]))
		{
			add_token(t, t->i - 1, t->i - 1, tokens, str, 0);
			t->prev = str[t->i];
		}
		else if (str[t->i] == '\\' || str[t->i] == '$' || str[t->i] == '`' ||\
				str[t->i] == '\'' || str[t->i] == '\"' || str[t->i] == '(' ||\
				str[t->i] == '~')
			quoting(str, t, tokens);
		else if (is_operator(str[t->i]))
		{
			if (t->prev && t->prev != ' ' && t->prev != '\t' && t->prev != '\n')
				add_token(t, t->i - 1, t->j, tokens, str, 0);
			t->prev = str[t->i];
			t->j = t->i;
		}
		else if (t->prev && (str[t->i] == ' ' || str[t->i] == '\t' || str[t->i] == '\n') && (t->prev != ' ' && t->prev != '\t' && t->prev != '\n'))
			add_token(t, t->i - 1, t->j, tokens, str, 0);
		else
		{
			(t->prev == ' ' || t->prev == '\t' || t->prev == '\n') ? t->prev = '\0' : 0;
			(!(t->prev)) ? t->j = t->i : 0;
		    t->prev = str[t->i];
		}
	}
}
