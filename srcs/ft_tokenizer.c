/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenizer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcarney <mcarney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/26 15:55:08 by mcarney           #+#    #+#             */
/*   Updated: 2018/11/11 11:03:49 by mcarney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void			add_token(t_info *t, int i, int j, char *str)
{
	t_ast			*new;
	t_ast			*old;

	if (i - j >= 0 && !(i - j == 0 && str[j] == ' '))
	{
		old = t->tokens;
		if (!(new = (t_ast *)malloc(sizeof(t_ast))))
			ft_restart(t->tokens, "Malloc error");
		new->val = ft_strsub(str, j, i - j + 1);
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
		new->expand = t->expand;
		new->operator = t->operator;
	}
	t->j = t->i;
	t->expand = 0;
	t->prev = '\0';
}

static void		sub_tokenize(char *str, t_info *t)
{
	if (OPERATOR)
	{
		if (t->prev && !(PREV_WHITESPACE))
			add_token(t, t->i - 1, t->j, str);
		t->prev = str[t->i];
		t->j = t->i;
		if (str[t->i] == ';')
		{
			t->operator = 1;
			add_token(t, t->i, t->j, str);
			t->operator = 0;
		}
	}
	else if (t->prev && (WHITESPACE) && !(PREV_WHITESPACE))
		add_token(t, t->i - 1, t->j, str);
	else
	{
		(PREV_WHITESPACE) ? t->prev = '\0' : 0;
		(!(t->prev)) ? t->j = t->i : 0;
		t->prev = str[t->i];
	}
}

void			tokenize(char *str, t_info *t, int len)
{
	while (str && len > ++t->i && str[t->i] != '#')
	{
		if (t->prev && (PREV_OPERATOR) && str[t->i] == t->prev)
		{
			t->operator = 1;
			add_token(t, t->i, t->i - 1, str);
			t->operator = 0;
		}
		else if (t->prev && (PREV_OPERATOR))
		{
			t->operator = 1;
			add_token(t, t->i - 1, t->i - 1, str);
			t->prev = str[t->i];
			t->operator = 0;
		}
		else if (SPECIAL_CHAR || QUOTE)
			quoting(str, t);
		else
			sub_tokenize(str, t);
	}
	(t->prev && !(PREV_WHITESPACE)) ? add_token(t, t->i - 1, t->j, str) : 0;
}
