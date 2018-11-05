/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_validate.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcarney <mcarney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/26 15:54:49 by mcarney           #+#    #+#             */
/*   Updated: 2018/11/04 19:04:49 by mcarney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int					count_backslashes(t_okenize *t, char *str)
{
	int				i;

	i = t->i - 1;
	while (str[i] && str[i] == '\\')
		i--;
	return ((t->i - i + 1) % 2);
}

int					ft_strfind(const char *s1, const char *s2)
{
	size_t			i;
	size_t			j;
	unsigned char	*str1;
	unsigned char	*str2;

	i = 0;
	j = 0;
	if (ft_strlen(s1) > ft_strlen(s2))
		return (0);
	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	while (str1[i] && str2[j])
	{
		if (str1[i] == str2[j])
		{
			i++;
			j++;
		}
		else
		{
			j = j - i + 1;
			i = 0;
		}
	}
	return (!(str1[i]));
}

void		free_ast(t_ast *tokens)
{
	t_ast	*tmp;

	if (!(tokens))
		return ;
	if (!(tokens->r_child))
	{
		while (tokens->l_child)
		{
			tmp = tokens;
			tokens = tokens->l_child;
			ft_strdel(&tmp->val);
			free(tmp);
		}
	}
	else
	{
		if (tokens->l_child)
			free_ast(tokens->l_child);
		if (tokens->r_child)
			free_ast(tokens->r_child);
	}
	ft_strdel(&tokens->val);
	free(tokens);
}

int			ft_len_cmd(t_ast *tokens)
{
	int		len;

	len = 0;
	while (tokens)
	{
		len++;
		tokens = tokens->l_child;
	}
	return (len);
}

void		append_str(char *str, t_okenize *t, t_ast **tokens, char *msg)
{
	char	*tmp;
	char	*line;

	t->i = -1;
	ft_putstr(msg);
	if (get_next_line(0, &line) <= 0)
		ft_error("gnl error");
	tmp = ft_strappend(str, '\n');
	tmp = free_join(tmp, line);
	ft_strdel(&line);
	check_quotes(tmp, t, tokens);
	free(tmp);
}

int			check_operator(char *str, t_okenize *t, t_ast **tokens)
{
	while (str[t->i])
		t->i++;
	t->i--;
	while (t->i >= 0 && (whitespace))
		t->i--;
	if (str[t->i] && str[t->i] != ';' && (operator) &&\
		!(count_backslashes(t, str)))
	{
		append_str(str, t, tokens, "operator> ");
		return (1);
	}
	return (0);
}

void		check_quotes(char *str, t_okenize *t, t_ast **tokens)
{
	char	ch;
	char	*tmp;

	while (str[++t->i])
		if (((quote) || (str[t->i] && str[t->i] == '$' &&\
			str[t->i + 1] == '(')) && !(count_backslashes(t, str)))
		{
			ch = (str[t->i++] != '(') ? str[t->i - 1] : ')';
			while (str[t->i] && (str[t->i] != ch || (str[t->i] == ch &&\
					count_backslashes(t, str))))
				t->i++;
			if (!(str[t->i]))
			{
				append_str(str, t, tokens, "quote> ");
				return ;
			}
		}
	t->i = 0;
	if (!(check_operator(str, t, tokens)))
	{
		t->i = -1;
		tmp = ft_check_history(str);
		ft_history_add(tmp);
		(tmp) ? tokenize(tmp, t, tokens, ft_strlen(tmp)) : 0;
		(t->prev && !(prev_whitespace)) ? add_token(t, t->i - 1, t->j, tokens, tmp, 0, 0) : 0;
		(tmp != str) ? ft_strdel(&tmp) : 0;
	}
}
