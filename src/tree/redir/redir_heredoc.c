#include "../../headers/minishell.h"

t_r_in	*test_open_heredoc(t_r_in *redir_in)
{
	char *tmp;

	tmp = get_file_name();
	redir_in->file = ft_strdup(tmp);
	free(tmp);
	redir_in->fd = open(redir_in->file, O_CREAT, 0644);
	if (redir_in->fd < 0)
		return (NULL);
	close(redir_in->fd);
	return (redir_in);
}

t_r_in	*do_heredoc(t_r_in *redir_in, t_token *tok, t_env *mini, t_tree *node)
{
	pid_t	proc;
	int		code;

	redir_in = test_open_heredoc(redir_in);
	if (!redir_in)
		return (NULL);
	redir_in->fd = open(redir_in->file, O_RDONLY, 0644);
	if (redir_in->fd < 0)
		return (NULL);
	proc = fork();
	if (proc < 0)
		return (NULL);
	else if (proc == 0)
	{
		redir_in->path = tok->data;
		redir_in->is_quote = tok->is_quote;
		fork_heredoc_n(mini, redir_in, node);
	}
	waitpid(proc, &code, 0);
	if (g_exit_code == 130)
	{
		(close(redir_in->fd), unlink(redir_in->file));
		redir_in->fd = -1;
	}
	return (redir_in);
}

int	file_can_open(char *path)
{
	if (!path)
		return (-1);
	if (access(path, R_OK | W_OK) != 0)
		return (0);
	return (1);
}

char	*get_file_name(void)
{
	int		fd;
	char	*file;
	char	buf[1001];
	int		i[2];

	fd = open("/dev/random", O_RDONLY);
	if (fd < 0 || (ft_memset(buf, 0, 1000) && read(fd, buf, 999) == -1))
		return (NULL);
	buf[999] = 0;
	file = malloc(sizeof(char) * 30);
	if (!file)
		return ((close(fd), NULL));
	i[0] = 0;
	i[1] = 0;
	while (buf[i[0]] && i[0] < 999 && i[1] < 29)
	{
		if (ft_isalnum(buf[i[0]]))
			file[i[1]++] = buf[i[0]];
		i[0]++;
	}
	if (i[1] < 15)
		return ((free(file), get_file_name()));
	file[i[1]] = 0;
	return (close(fd), (file));
}