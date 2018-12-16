#include "shell.h"

static char		*get_history(char *cmd)
{
	int			i;
	int			index;
	t_history	*tmp;

	i = -1;
	if (cmd[0] == '!' && cmd[1] && cmd[1] != '\'' && cmd[1] != '\"' &&
		!ISSPACE(cmd[1]) && !ISSPACE2(cmd[1]))
	{
		index = (cmd[1] == '!') ? -1 : ft_atoi(&cmd[1]);
		(g_history && index < 0) && (index = g_history->index + index + 1);
		if (!g_history || index <= 0 || index > g_history->index)
		{
			if (index == 0)
				ft_printf("42sh: event not found: %s\n", &cmd[1]);
			else
				ft_printf("42sh: event not found: %d\n", ft_atoi(&cmd[1]));
			return (NULL);
		}
		tmp = g_history;
		while (tmp->index != index)
			tmp = tmp->next;
		return (tmp->command);
	}
	return ("!");
}

static int		ft_check_history_helper(char *str, int mark)
{
	if (!ft_strncmp(&str[mark], "!!", 2))
		mark += 2;
	else if (ft_numlen(&str[mark + 1]) != 0)
		mark = mark + ft_numlen(&str[mark + 1]) + 1;
	else
		mark += 1;
	return (mark);
}

static char		*get_history_command(char *str, char *cmd, int mark, int t_mark)
{
	char		*tmp;

	tmp = ft_strsub(str, t_mark, mark - t_mark);
	cmd = (cmd) ? free_join(cmd, tmp) : ft_strdup(tmp);
	(tmp) ? ft_strdel(&tmp) : 0;
	tmp = (str[mark] == '!') ? get_history((str + mark)) : ft_strdup("");
	cmd = (cmd) ? free_join(cmd, tmp) : ft_strdup(tmp);
	return (cmd);
}

static int		move_mark_in_siglequote(char *str, int mark)
{
	while (str[++mark] && (str[mark] != '\'' ||
				(str[mark] == '\'' && count_backslashes(mark, str))))
		;
	mark++;
	return (mark);
}

char			*ft_check_history(char *str)
{
	int			mark;
	int			t_mark;
	char		*cmd;

	if (!ft_strchr(str, '!'))
		return (str);
	cmd = NULL;
	mark = 0;
	t_mark = mark;
	while (str && str[mark])
	{
		if (str[mark] == '\'' && !(count_backslashes(mark, str)))
			mark = move_mark_in_siglequote(str, mark);
		else if (str[mark] == '!' && !(count_backslashes(mark, str)))
		{
			cmd = get_history_command(str, cmd, mark, t_mark);
			mark = ft_check_history_helper(str, mark);
			t_mark = mark;
		}
		else
			mark++;
	}
	cmd = (cmd) ? free_join(cmd, &str[t_mark]) : ft_strdup(str);
	(ft_strcmp(cmd, str) != 0) ? (ft_putendl(cmd)) : 0;
	return (cmd);
}
