/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenizer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcarney <mcarney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/26 15:55:08 by mcarney           #+#    #+#             */
/*   Updated: 2018/11/02 16:35:28 by mcarney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void				add_token(t_okenize *t, int i, int j, t_ast **tokens, char *str, int expand, int prev)
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
	t->prev = (prev) ? str[t->i] : '\0';
	new->expand = expand;
}

void				quoting(char *str, t_okenize *t, t_ast **tokens)
{
	char	ch;
	int		expand;
	char	*tmp;
	char	*tmp2;
	int		i;
	int		j;

	expand = 0;
	ch = (str[t->i] != '(') ? str[t->i] : ')';
	if (ch == '\\')
	{
		i = t->i;
		j = i;
		while (str[i] && str[i] == '\\')
			i++;
		i = i - t->i;
		expand = (i % 2) ? 0 : 1;
		i = (i > 1) ? i / 2 + !(expand) : 1;
		(!(t->prev)) ? t->j = t->i + i : 0;
		(str[++t->i] && (whitespace)) ? t->i++ : 0;
		while (str[t->i] && !(whitespace) && str[t->i] != '\\')
			t->i++;
		if (t->prev)
		{
			tmp = ft_strsub(str, t->j, j - t->j);
			tmp2 = ft_strsub(str, j + i, t->i - (j + i));
			tmp = free_join(tmp, tmp2);
			add_token(t, ft_strlen(tmp), 0, tokens, tmp, expand, 0);
			free(tmp);
			free(tmp2);
		}
		else
			add_token(t, t->i - 1, t->j, tokens, str, expand, 0);
		return ;
	}
	else if (ch == '$' || ch == '~')
	{
		expand = 1;
		(!(t->prev)) ? t->j = t->i : 0;
		if (str[t->i + 1] && str[t->i + 1] == '(')
		{
			expand = 2;
			while (str[t->i] && (str[t->i] != ')' ||\
					(str[t->i] == ')' && str[t->i - 1] == '\\')))
				t->i++;
		}
		else
			while (str[t->i] && !(whitespace) && !(quote) && str[t->i] != '\'')
				t->i++;
	}
	else
	{
		if (t->prev && !(prev_whitespace))
			add_token(t, t->i - 1, t->j, tokens, str, 0, 0);
		t->j = t->i++;
		while (str[t->i] && (str[t->i] != ch ||\
				(str[t->i] == ch && str[t->i - 1] == '\\')))
		{
			(ch == '"' && str[t->i - 1] != '\\' && str[t->i] == '$') ? expand = 1 : 0;
			t->i++;
		}
	}
	if (ch == '`' || expand == 2)
	{
		(!(expand)) ? expand = 1 : 0;
		tmp = ft_strsub(str, t->j + expand, t->i - (t->j + expand));
		tmp2 = get_backquote(tmp);
		(tmp2) ? add_token(t, ft_strlen(tmp2), 0, tokens, tmp2, 0, 0) : 0;
		(tmp2) ? free(tmp2) : 0;
	}
	else if (ch == '\'' || ch == '\"')
		add_token(t, t->i - 1, t->j + 1, tokens, str, expand, 0);
	else
		add_token(t, t->i - 1, t->j, tokens, str, expand, 0);
}

int					is_operator(char a)
{
	if (a == '|' || a == '&' || a == ';' || a == '<' || a == '>')
		return (1);
	return (0);
}

void 				tokenize(char *str, t_okenize *t, t_ast **tokens, int len)
{
	while (str && len > ++t->i && str[t->i] != '#')
	{
		if (t->prev && is_operator(t->prev) && str[t->i] == t->prev)
			add_token(t, t->i, t->i - 1, tokens, str, 0, 0);
		else if (t->prev && is_operator(t->prev))
			add_token(t, t->i - 1, t->i - 1, tokens, str, 0, 1);
		else if (special_char || quote )
			quoting(str, t, tokens);
		else if (is_operator(str[t->i]))
		{
			if (t->prev && !(prev_whitespace))
				add_token(t, t->i - 1, t->j, tokens, str, 0, 1);
			t->prev = str[t->i];
			t->j = t->i;
		}
		else if (t->prev && (whitespace) && !(prev_whitespace))
			add_token(t, t->i - 1, t->j, tokens, str, 0, 0);
		else
		{
			(prev_whitespace) ? t->prev = '\0' : 0;
			(!(t->prev)) ? t->j = t->i : 0;
			t->prev = str[t->i];
		}
	}
}
