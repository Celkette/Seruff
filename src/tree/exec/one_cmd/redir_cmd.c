/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 02:57:22 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/28 22:08:33 by Seth         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

int	use_redir_in_here_doc(t_tree *node)
{
	if (node->redir_in->type == 2)
	{
		if (node->redir_in->fd < 0)
		{
			ft_putstr_fd(strerror(errno), 2);
			return (-1);
		}
		if (dup2(node->redir_in->fd, STDIN_FILENO) < 0)
		{
			close(node->redir_in->fd);
			ft_putstr_fd(strerror(errno), 2);
			return (-3);
		}
		close(node->redir_in->fd);
		unlink(node->redir_in->file);
		return (0);
	}
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
			return (errno);
		}
		if (dup2(fd, STDIN_FILENO) < 0)
		{
			close(fd);
			ft_putstr_fd(strerror(errno), 2);
			return (-3);
		}
		close(fd);
		return (0);
	}
	return (use_redir_in_here_doc(node));
}

static int	get_redir_fd_out(t_r_out *redir_out)
{
	int	fd;
	int	fd_to_redirect;

	if (redir_out->type == 2)
		fd = open(redir_out->path, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(redir_out->path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
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
		close(fd);
		printf("%s\n", strerror(errno));
		return (-2);
	}
	return (fd);
}

int	last_redir(t_tree *node, t_list *redir_list)
{
	if (!node->top && (((t_r_out *)redir_list->data)->fd == 0))
	{
		node->stdin_cpy = 1;
		node->status = dup(STDIN_FILENO);
	}
	else if (!node->top && (((t_r_out *)redir_list->data)->fd == -1 || \
	((t_r_out *)redir_list->data)->fd == 1))
	{
		node->stdout_cpy = 1;
		node->status = dup(STDOUT_FILENO);
	}
	return (get_redir_fd_out(redir_list->data));
}

int	use_redir_out(t_tree *node)
{
	t_r_out		*redir_out;
	t_list		*redir_list;
	int			fd;

	if (!node)
		return (-1);
	fd = 0;
	redir_list = node->redir_out;
	node->status = -1;
	while (redir_list)
	{
		redir_out = (t_r_out *)redir_list->data;
		if (access(redir_out->path, R_OK) != 0)
			return (-1);
		if (fd < 0)
			return (fd);
		if (!redir_list->next)
		{
			fd = last_redir(node, redir_list);
			break ;
		}
		redir_list = redir_list->next;
	}
	close(fd);
	return (0);
}
