/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_validate.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcarney <mcarney@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/26 15:54:49 by mcarney           #+#    #+#             */
/*   Updated: 2018/10/26 19:47:40 by fhong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void 		defaults(t_okenize *t)
{
	t->i = -1;
	t->j = 0;
	t->tokens = 0;
	t->prev = '\0';
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

void	free_ast(t_ast *tokens)
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

int		ft_len_cmd(t_ast *tokens)
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

void 		append_str(char *str, t_okenize *t, t_ast **tokens, char *msg)
{
	char *tmp;
	char *line;

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

int		check_operator(char *str, t_okenize *t, t_ast **tokens)
{
	while (str[t->i])
		t->i++;
	t->i--;
	while (t->i >= 0 && (str[t->i] == ' ' || str[t->i] == '\t'))
		t->i--;
	if (t->i >= 0 && ((is_operator(str[t->i]) && t->i == 0) ||\
		(t->i >= 1 && str[t->i - 1] != '\\' && is_operator(str[t->i]))))
	{
		append_str(str, t, tokens, "operator> ");
		return (1);
	}
	return (0);
}

void	check_quotes(char *str, t_okenize *t, t_ast **tokens)
{
	char ch;

	while (str[++t->i])
		if (((str[t->i] == '\'' || str[t->i] == '\"') && t->i == 0) ||\
			((t->i >= 1 && str[t->i - 1] != '\\') &&\
			(str[t->i] == '\'' || str[t->i] == '\"')))
		{
			ch = str[t->i++];
			while (str[t->i] && (str[t->i] != ch || (str[t->i] == ch &&\
					(t->i >= 1 && str[t->i - 1] == '\\'))))
				t->i++;
			if (!(str[t->i]))
			{
				append_str(str, t, tokens, "quote> ");
				return ;
			}
		}
	if (!(check_operator(str, t, tokens)))
	{
		t->i = -1;
		str = ft_check_history(str);
		ft_history_add(str);
		tokenize(str, t, tokens);
		if (t->prev && t->prev != ' ' && t->prev != '\t')
			add_token(t, t->i, t->j, tokens, str);
		ft_strdel(&str);
	}
}
