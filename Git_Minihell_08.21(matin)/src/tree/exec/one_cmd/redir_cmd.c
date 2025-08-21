/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaporte <nlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 02:57:22 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/13 18:26:22 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

int	use_redir_in_here_doc(t_tree *node)
{
	pid_t	fork_pid;
	int		len;
	char	*path_p;

	path_p = node->redir_in->path;
	if (pipe(node->here_doc_fd) < 0)
		return (print_error_pipeline("can't pipe:", strerror(errno), -1));
	if (dup2(node->here_doc_fd[0], STDIN_FILENO) < 0)
		return (print_error_pipeline("can 't dup2:", strerror(errno), -1));
	len = ft_strlen(path_p);
	if (len < 4000)
	{
		write(node->here_doc_fd[1], path_p, len);
		close(node->here_doc_fd[1]);
		return (0);
	}
	fork_pid = fork();
	if (fork_pid < 0)
		return (print_error_pipeline("can't fork:", strerror(errno), -1));
	use_redir_in_here_doc2(node, fork_pid, path_p);
	close(node->here_doc_fd[1]);
	return (0);
}

// Gere la redirection infile
int	use_redir_in(t_tree *node)
{
	int	fd;

	if (!node)
		return (-1);
	if (node->redir_in->type == 1)
	{
		fd = open(node->redir_in->path, O_RDONLY);
		if (fd < 0)
		{
			ft_putstr_fd(strerror(errno), 2);
			return (1);
		}
		if (dup2(fd, STDIN_FILENO) < 0)
		{
			ft_putstr_fd(strerror(errno), 2);
			return (-3);
		}
		return (0);
	}
	return (use_redir_in_here_doc(node));
}

static int	get_redir_fd_out(t_redir_out *redir_out)
{
	int	fd;
	int	fd_to_redirect;

	if (redir_out->type == 2)
		fd = open(redir_out->path, O_APPEND | O_WRONLY);
	else
		fd = open(redir_out->path, O_TRUNC | O_WRONLY);
	if (fd < 0)
	{
		perror("open");
		printf("fd %s\n", strerror(errno));
		return (-1);
	}
	if (redir_out->fd < 0)
		fd_to_redirect = STDOUT_FILENO;
	else
		fd_to_redirect = redir_out->fd;
	if (dup2(fd, fd_to_redirect) == -1)
	{
		printf("%s\n", strerror(errno));
		return (-2);
	}
	return (fd);
}

// Fere le redirection out
int	use_redir_out(t_tree *node)
{
	t_redir_out	*redir_out;
	t_list		*redir_list;
	int			fd;

	if (!node)
		return (-1);
	redir_list = node->redir_out;
	while (redir_list)
	{
		redir_out = (t_redir_out *)redir_list->data;
		fd = get_redir_fd_out(redir_out);
		if (fd < 0)
			return (fd);
		redir_list = redir_list->next;
	}
	close(fd);
	return (0);
}
