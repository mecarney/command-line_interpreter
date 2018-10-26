#include "shell.h"

t_ast		*search(t_ast **tokens, int *n, char *str, size_t len)
{
	t_ast	*old;
	t_ast	*prev;

	old = *tokens;
	prev = NULL;
	while (n && old)
	{
		if (ft_strlen(old->val) >= len && ft_strfind(old->val, str))
		{
			if (prev)
				prev->l_child = NULL;
			else
				ft_error("parse error"); //manage leaks and make sure this is true
			*n = 0;
			return (old);
		}
		prev = old;
		old = old->l_child;
	}
	return (*tokens);
}

t_ast 		*parser(t_ast **tokens, t_ast *parent)
{
	t_ast	*old;
	t_ast	*prev;
	int		n;

	if (!(*tokens))
		return (NULL);
	n = 1;
	old = *tokens;
	prev = NULL;
	old = search(tokens, &n, ";", 1);
	(n) ? old = search(tokens, &n, "&&||", 2) : 0;
	(n) ? old = search(tokens, &n, "&|>><<", 1) : 0;
	if (n == 0)
	{
		old->r_child = parser(&old->l_child, old);
		old->l_child = parser(tokens, old);
	}
	else
		old = *tokens;
	old->parent = (parent) ? parent : NULL;
	return (old);
}

void	add_token(t_okenize *t, int i, int j, t_ast **tokens, char *str)
{
	t_ast *new;
	t_ast *old;

	old = *tokens;
	if (!(new = (t_ast *)malloc(sizeof(t_ast))))
		ft_error("Malloc error");
	new->val = ft_strsub(str, j, i - j + 1);
	new->l_child = NULL;
	new->r_child = NULL;
	// ft_printf("%s\n", new->val);
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

void				quoting(char *str, char ch, t_okenize *t, t_ast **tokens)
{
	t->j = t->i;
	t->i = t->i + 1;
	while (str[t->i] && str[t->i] != ch)
		t->i = t->i + 1; //check for $ with "
	add_token(t, t->i, t->j, tokens, str);
}

int					is_operator(char a)
{
	if (a == '|' || a == '&' || a == ';' || a == '<' || a == '>')// || a == '(')
		return (1);
	return (0);
}

void 	tokenize(char *str, t_okenize *t, t_ast **tokens)
{
	while (str[++t->i])
	{
		if (t->prev && is_operator(t->prev) && str[t->i] == t->prev)
			add_token(t, t->i, t->i - 1, tokens, str);
		else if (t->prev && is_operator(t->prev) && is_operator(str[t->i]))
			add_token(t, t->i - 1, t->i - 1, tokens, str);
		// else if (str[t->i] == '\\' || str[t->i] == '$' || str[t->i] == '`')
		// 	store for expansion();
		else if (str[t->i] == '\'' || str[t->i] == '\"')
			quoting(str, str[t->i], t, tokens);
		else if (is_operator(str[t->i]))
		{
			t->prev = str[t->i];
			t->j = t->i;
		}
		else if (t->prev && (str[t->i] == ' ' || str[t->i] == '\t') && (t->prev != ' ' && t->prev != '\t'))
			add_token(t, t->i - 1, t->j, tokens, str);
		else if (str[t->i] == '#')
			break ;
		else
		{
			(t->prev == ' ' || t->prev == '\t') ? t->prev = '\0' : 0;
			(!(t->prev)) ? t->j = t->i : 0;
		    t->prev = str[t->i];
		}
	}
}

void 		defaults(t_okenize *t)
{
	t->i = -1;
	t->j = 0;
	t->tokens = 0;
	t->prev = '\0';
}

void print_ast(t_ast *tokens)
{
	if (!(tokens))
		return ;
	if (!(tokens->r_child))
	{
		ft_printf("command: %s\n", tokens->val);
		while (tokens->l_child)
		{
			ft_printf("command: %s\n", tokens->l_child->val);
			ft_strdel(&tokens->val);
			free(tokens);
			tokens = tokens->l_child;
		}
	}
	else
	{
		if (tokens->l_child)
		{
			ft_printf("L: %s\n", tokens->l_child->val);
			print_ast(tokens->l_child);
		}
		if (tokens->r_child)
		{
			ft_printf("R: %s\n", tokens->r_child->val);
			print_ast(tokens->r_child);
		}
	}
	ft_strdel(&tokens->val);
	free(tokens);
}

void 		append_str(char *str, t_okenize *t, t_ast **tokens, char *msg)
{
	char *tmp;
	char *line;

	t->i = -1;
	ft_putstr(msg);
	if (get_next_line(0, &line) <= 0)
		ft_error("gnl error");
	tmp = ft_strjoin(str, "\n");
	str = ft_strdup(tmp);
	free(tmp);
	tmp = ft_strjoin(str, line);
	str = ft_strdup(tmp);
	free(tmp);
	ft_strdel(&line);
	check_quotes(str, t, tokens);
	free(str);
}

int		check_operator(char *str, t_okenize *t, t_ast **tokens)
{
	while (str[t->i])
		t->i++;
	t->i--;
	while (str[t->i] == ' ' || str[t->i] == '\t')
		t->i--;
	if (is_operator(str[t->i]))
	{
		append_str(str, t, tokens, "operator> ");
		return (1);
	}
	ft_history_add(str);
	return (0);
}

void	check_quotes(char *str, t_okenize *t, t_ast **tokens)
{
	char ch;

	while (str[++t->i])
		if (str[t->i] == '\'' || str[t->i] == '\"')
		{
			ch = str[t->i++];
			while (str[t->i] && str[t->i] != ch)
				t->i++;
			if (!(str[t->i]))
			{
				append_str(str, t, tokens, "quote> ");
				return ;
			}
		}
	if (check_operator(str, t, tokens))
		return ;
	defaults(t);
	tokenize(str, t, tokens);
	if (t->prev && t->prev != ' ' && t->prev != '\t')
		add_token(t, t->i, t->j, tokens, str);
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
