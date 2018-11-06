/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenizer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcarney <mcarney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/26 15:55:08 by mcarney           #+#    #+#             */
/*   Updated: 2018/11/06 13:45:29 by mcarney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void				add_token(t_okenize *t, int i, int j, char *str)
{
	t_ast			*new;
	t_ast			*old;

	old = t->tokens;
	if (!(new = (t_ast *)malloc(sizeof(t_ast))))
		ft_error("Malloc error");
	new->val = ft_strsub(str, j, i - j + 1);
	ft_printf("|%s|\n", new->val);
	new->l_child = NULL;
	new->r_child = NULL;
	if (!(t->tokens))
		t->tokens = new;
	else
	{
		while (old->l_child)
			old = old->l_child;
		old->l_child = new;
	}
	t->j = t->i;
	new->expand = t->expand;
	t->expand = 0;
}

void				handle_backslash(char *str, t_okenize *t)
{
	t->number_bs = t->i;
	t->bs_index = t->i;
	while (str[t->number_bs] && str[t->number_bs] == '\\')
		t->number_bs++;
	t->number_bs -= t->i;
	t->expand = !(t->number_bs % 2);
	t->i += t->number_bs;
	t->number_bs = (t->number_bs > 1) ? t->number_bs / 2 + !(t->expand) : 1;
	(!(t->prev)) ? t->j = t->i - t->number_bs : 0;
	(str[++t->i] && (whitespace)) ? t->i++ : 0;
	while (str[t->i] && !(whitespace || quote || special_char))
		t->i++;
	(whitespace || quote || special_char) ? t->i-- : 0;
}

void				handle_dollar_tilde(char *str, t_okenize *t)
{
	char			*tmp;
	char			*tmp2;

	t->expand = 1;
	(!(t->prev)) ? t->j = t->i : 0;
	if (str[t->i] == '$' && str[t->i + 1] && str[t->i + 1] == '(')
	{
		t->expand = 2;
		while (str[t->i] && (str[t->i] != ')' ||\
				(str[t->i] == ')' && (count_backslashes(t, str)))))
			t->i++;
		tmp = ft_strsub(str, t->j + t->expand, t->i - (t->j + t->expand));
		tmp2 = get_backquote(tmp);
		(tmp2) ? add_token(t, ft_strlen(tmp2), 0, tmp2) : 0;
		(tmp2) ? free(tmp2) : 0;
	}
	else
	{
		while (str[t->i] && !(whitespace || quote) &&\
				!(count_backslashes(t, str)))
			t->i++;
		add_token(t, t->i - 1, t->j, str);
	}
}

void				handle_quotation(char *str, t_okenize *t)
{
	char			ch;
	char			*tmp;
	char			*tmp2;

	ch = str[t->i];
	if (t->prev && !(prev_whitespace))
		add_token(t, t->i - 1, t->j, str);
	t->j = ++t->i;
	t->prev = t->i;
	while (str[t->i] && (str[t->i] != ch ||\
			(str[t->i] == ch && (count_backslashes(t, str)))))
		((ch == '"' || ch == '`') && (special_char || quote)) ?\
			quoting(str, t) : t->i++;
	if (ch == '`')
	{
		tmp = ft_strsub(str, t->j + t->expand, t->i - (t->j + t->expand));
		tmp2 = get_backquote(tmp);
		(tmp2) ? add_token(t, ft_strlen(tmp2), 0, tmp2) : 0;
		(tmp2) ? free(tmp2) : 0;
	}
	else if (ch == '\'' || ch == '\"')
		add_token(t, t->i - 1, t->j, str);
}

void				quoting(char *str, t_okenize *t)
{
	char			ch;
	char			*tmp;
	char			*tmp2;

	ch = str[t->i];
	if (ch == '\\')
		handle_backslash(str, t);
	else if (ch == '$' || ch == '~')
		handle_dollar_tilde(str, t);
	else
		handle_quotation(str, t);
	if (ch == '\\' && t->prev)
	{
		tmp = ft_strsub(str, t->j, t->bs_index - t->j);
		tmp2 = ft_strsub(str, t->bs_index + t->number_bs,\
						t->i - (t->bs_index + t->number_bs) + 1);
		tmp = free_join(tmp, tmp2);
		add_token(t, ft_strlen(tmp), 0, tmp);
		free(tmp);
		free(tmp2);
	}
	else if (ch == '\\')
		add_token(t, t->i, t->j + !(t->expand), str);
	t->prev = '\0';
}
