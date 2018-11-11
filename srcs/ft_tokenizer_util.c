/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenizer_util.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcarney <mcarney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/11 09:07:27 by mcarney           #+#    #+#             */
/*   Updated: 2018/11/11 10:08:59 by mcarney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int				count_backslashes(int index, char *str)
{
	int				i;

	i = index - 1;
	while (i >= 0 && str[i] == '\\')
		i--;
	return ((index - i + 1) % 2);
}

static void		handle_dollar_tilde(char *str, t_info *t)
{
	char			*tmp;

	t->expand = !(count_backslashes(t->i, str));
	(!(t->prev)) ? t->j = t->i : 0;
	if (str[t->i] == '$' && !(count_backslashes(t->i, str)) &&\
		str[t->i + 1] && str[t->i + 1] == '(')
	{
		t->i++;
		while (str[t->i] && (str[t->i] != ')' ||\
				(str[t->i] == ')' && (count_backslashes(t->i, str)))))
			t->i++;
		t->expand = 2;
		tmp = ft_strsub(str, t->j + 2, t->i - (t->j + 2));
		add_token(t, t->i - (t->j + 2), 0, tmp);
		free(tmp);
	}
	else
	{
		while (str[t->i] && !(WHITESPACE || QUOTE) &&\
				!(count_backslashes(t->i, str)))
			t->i++;
		add_token(t, t->i - 1, t->j, str);
	}
}

static void		handle_backslash(char *str, t_info *t)
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
	if (t->expand && str[t->i] && str[t->i] == '$' &&\
		str[t->i + 1] && str[t->i + 1] == '(')
		t->i--;
	else
	{
		(str[++t->i] && (WHITESPACE)) ? t->i++ : 0;
		while (str[t->i] && !(WHITESPACE || QUOTE || SPECIAL_CHAR))
			t->i++;
		(WHITESPACE || QUOTE || SPECIAL_CHAR) ? t->i-- : 0;
	}
}

static void		handle_quotation(char *str, t_info *t)
{
	char			ch;
	char			*tmp;

	ch = str[t->i];
	t->j = ++t->i;
	while (str[t->i] && (str[t->i] != ch))
	{
		t->prev = str[t->i];
		if ((ch == '"' || ch == '`') && (SPECIAL_CHAR || str[t->i] == '`'))
		{
			quoting(str, t);
			t->j = t->i + 1;
			(str[t->i] == ch) ? t->i-- : 0;
		}
		t->i++;
	}
	if (ch == '`')
	{
		t->expand = 2;
		tmp = ft_strsub(str, t->j, t->i - (t->j));
		add_token(t, t->i - t->j, 0, tmp);
		free(tmp);
	}
	else if (ch == '\'' || ch == '\"')
		add_token(t, t->i - 1, t->j, str);
}

void			quoting(char *str, t_info *t)
{
	char			ch;
	char			*tmp;
	char			*tmp2;

	(t->prev) ? add_token(t, t->i - 1, t->j, str) : 0;
	t->j = t->i + 1;
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
}
