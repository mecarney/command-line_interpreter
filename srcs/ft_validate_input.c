#include "shell.h"

static void		append_str(char *str, t_info *t, t_ast **tokens, char *msg)
{
	char	*tmp;
	char	*line;

	tmp = NULL;
	ft_putstr_fd(msg, 2);
	if (get_next_line(0, &line) <= 0)
		ft_restart(*tokens, "read error");
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

static int		check_operator(char *str, t_info *t, t_ast **tokens)
{
	char	*tmp;

	while (str[t->i])
		t->i++;
	t->i--;
	while (t->i >= 0 && (WHITESPACE))
		t->i--;
	if (str[t->i] && str[t->i] != ';' && (OPERATOR || str[t->i] == '\\') &&\
		!(count_backslashes(t->i, str)))
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

void			check_quotes(char *str, t_info *t, t_ast **tokens)
{
	char	ch;

	while (str[++t->i])
		if (((QUOTE) || (str[t->i] && str[t->i] == '$' &&\
			str[t->i + 1] == '(')) && !(count_backslashes(t->i, str)))
		{
			ch = (str[++t->i] != '(') ? str[t->i - 1] : ')';
			if (ch == '\'')
				while (str[t->i] && str[t->i] != ch)
					t->i++;
			else
				while (str[t->i] && (str[t->i] != ch || (str[t->i] == ch &&\
						count_backslashes(t->i, str))))
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
