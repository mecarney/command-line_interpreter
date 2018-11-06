/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_validate.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcarney <mcarney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/26 15:54:49 by mcarney           #+#    #+#             */
/*   Updated: 2018/11/05 15:25:03 by mcarney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void		sub_tokenize(char *str, t_okenize *t)
{
	if (operator)
	{
		if (t->prev && !(prev_whitespace))
		{
			add_token(t, t->i - 1, t->j, str);
			t->prev = str[t->i];
		}
		t->prev = str[t->i];
		t->j = t->i;
	}
	else if (t->prev && (whitespace) && !(prev_whitespace))
	{
		add_token(t, t->i - 1, t->j, str);
		t->prev = '\0';
	}
	else
	{
		(prev_whitespace) ? t->prev = '\0' : 0;
		(!(t->prev)) ? t->j = t->i : 0;
		t->prev = str[t->i];
	}
}

void		tokenize(char *str, t_okenize *t, int len)
{
	while (str && len > ++t->i && str[t->i] != '#')
	{
		if (t->prev && (prev_operator) && str[t->i] == t->prev)
		{
			add_token(t, t->i, t->i - 1, str);
			t->prev = '\0';
		}
		else if (t->prev && (prev_operator))
		{
			add_token(t, t->i - 1, t->i - 1, str);
			t->prev = str[t->i];
		}
		else if (special_char || quote)
			quoting(str, t);
		else
			sub_tokenize(str, t);
	}
	if (t->prev && !(prev_whitespace))
	{
		add_token(t, t->i - 1, t->j, str);
		t->prev = '\0';
	}
}

void		append_str(char *str, t_okenize *t, t_ast **tokens, char *msg)
{
	char	*tmp;
	char	*line;

	tmp = NULL;
	ft_putstr(msg);
	if (get_next_line(0, &line) <= 0)
		ft_error("gnl error");
	if (str[t->i] != '\\')
	{
		tmp = ft_strappend(str, '\n');
		tmp = free_join(tmp, line);
		ft_strdel(&line);
	}
	else
	{
		tmp = ft_strsub(str, 0, t->i);
		tmp = free_join(tmp, line);
		ft_strdel(&line);
	}
	t->i = -1;
	check_quotes(tmp, t, tokens);
	free(tmp);
}

int			check_operator(char *str, t_okenize *t, t_ast **tokens)
{
	char	*tmp;

	while (str[t->i])
		t->i++;
	t->i--;
	while (t->i >= 0 && (whitespace))
		t->i--;
	if (str[t->i] && str[t->i] != ';' && (operator || str[t->i] == '\\') &&\
		!(count_backslashes(t, str)))
	{
		ft_printf("%c ", str[t->i]);
		append_str(str, t, tokens, "operator> ");
		return (1);
	}
	t->i = -1;
	tmp = ft_check_history(str);
	ft_history_add(tmp);
	(tmp) ? tokenize(tmp, t, ft_strlen(tmp)) : 0;
	(tmp != str) ? ft_strdel(&tmp) : 0;
	return (0);
}

void		check_quotes(char *str, t_okenize *t, t_ast **tokens)
{
	char	ch;

	while (str[++t->i])
		if (((quote) || (str[t->i] && str[t->i] == '$' &&\
			str[t->i + 1] == '(')) && !(count_backslashes(t, str)))
		{
			ch = (str[++t->i] != '(') ? str[t->i - 1] : ')';
			while (str[t->i] && (str[t->i] != ch || (str[t->i] == ch &&\
					count_backslashes(t, str))))
				t->i++;
			if (!(str[t->i]))
			{
				ft_printf("%c ", ch);
				append_str(str, t, tokens, "quote> ");
				return ;
			}
		}
	t->i = 0;
	check_operator(str, t, tokens);
}
