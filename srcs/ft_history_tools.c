#include "shell.h"

t_history			*g_history;
char				*g_history_file;

static t_history	*new_history(char *command)
{
	t_history	*new_hist;

	new_hist = (t_history *)malloc(sizeof(t_history) * 1);
	new_hist->index = 0;
	new_hist->command = ft_strdup(command);
	new_hist->next = NULL;
	return (new_hist);
}

void				ft_history_add(char *command)
{
	int			i;
	t_history	*new_hist;

	i = 1;
	if (!command || !*command)
		return ;
	new_hist = new_history(command);
	if (!g_history)
	{
		g_history = new_hist;
		new_hist->index = 1;
	}
	else
	{
		new_hist->next = g_history;
		new_hist->index = g_history->index + 1;
		g_history = new_hist;
	}
}

void				ft_print_history(t_history *history, int index, int io)
{
	t_history	*tmp;

	tmp = (history == g_history) ? g_history : history;
	if (tmp->next && index > 1)
		ft_print_history(tmp->next, index - 1, io);
	if (io == STDOUTPUT)
		ft_printf("%4d\t%s\n", tmp->index, tmp->command, io);
	else
		ft_printf("%s\n", tmp->command, io);
}

void				ft_write_history(void)
{
	int			fd;
	int			fd_base;
	t_history	*tmp;

	if (!g_history)
		return ;
	tmp = g_history;
	fd = open(g_history_file, O_WRONLY | O_CREAT, 0666);
	fd_base = dup(1);
	dup2(fd, 1);
	lseek(fd, 0, SEEK_END);
	ft_print_history(tmp, tmp->index, 0);
	dup2(fd_base, 1);
	close(fd);
}

void				ft_free_history(void)
{
	t_history	*tmp;

	while (g_history)
	{
		tmp = g_history;
		ft_strdel(&g_history->command);
		g_history = g_history->next;
		free(tmp);
	}
}
