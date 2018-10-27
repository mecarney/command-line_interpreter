/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenizer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcarney <mcarney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/26 15:55:08 by mcarney           #+#    #+#             */
/*   Updated: 2018/10/27 10:34:46 by mcarney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void				add_token(t_okenize *t, int i, int j, t_ast **tokens, char *str)
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
}

void				quoting(char *str, t_okenize *t, t_ast **tokens)
{
	char	ch;

	ch = str[t->i];
	if (t->prev && t->prev != ' ' && t->prev != '\t' && t->prev != '\n')
		add_token(t, t->i - 1, t->j, tokens, str);
	t->j = t->i++;
	if (ch == '\\')
		t->j = t->i;
	else if (ch == '$')
	{
		t->j = (t->i > 0) ? t->i - 1 : 0;
		while (str[t->i] && str[t->i] !=  ' ' && str[t->i] != '\t' && str[t->i] != '\n')
			t->i++;
	}
	// else if (ch == '`')
	else
	{
		while (str[t->i] && (str[t->i] != ch ||\
				(str[t->i] == ch && str[t->i - 1] == '\\')))
			t->i++; //check for $ with "
	}
	add_token(t, t->i, t->j, tokens, str);
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
			add_token(t, t->i, t->i - 1, tokens, str);
		else if (t->prev && is_operator(t->prev))// && is_operator(str[t->i]))
		{
			add_token(t, t->i - 1, t->i - 1, tokens, str);
			t->prev = str[t->i];
		}
		else if (str[t->i] == '\\' || str[t->i] == '$' || str[t->i] == '`' ||\
				str[t->i] == '\'' || str[t->i] == '\"')// || str[t->i] == '(')
			quoting(str, t, tokens);
		else if (is_operator(str[t->i]))
		{
			if (t->prev && t->prev != ' ' && t->prev != '\t' && t->prev != '\n')
				add_token(t, t->i - 1, t->j, tokens, str);
			t->prev = str[t->i];
			t->j = t->i;
		}
		else if (t->prev && (str[t->i] == ' ' || str[t->i] == '\t' || str[t->i] == '\n') && (t->prev != ' ' && t->prev != '\t' && t->prev != '\n'))
			add_token(t, t->i - 1, t->j, tokens, str);
		else
		{
			(t->prev == ' ' || t->prev == '\t' || t->prev == '\n') ? t->prev = '\0' : 0;
			(!(t->prev)) ? t->j = t->i : 0;
		    t->prev = str[t->i];
		}
	}
}
