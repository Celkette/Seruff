/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaporte <nlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 02:57:22 by nlaporte          #+#    #+#             */
/*   Updated: 2025/07/27 11:28:49 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

// Gere la redirection infile
int	use_redir_in(t_tree *node)
{
	int	fd;

	if (!node)
		return (-1);
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

// Fere le redirection out
int	use_redir_out(t_tree *node)
{
	t_redir_out	*redir_out;
	t_list		*redir_list;
	int	fd;

	if (!node)
		return (-1);
	redir_list = node->redir_out;
	while (redir_list)
	{
		redir_out = (t_redir_out *)redir_list->data;
		fd = -1;
		fd = open(redir_out->path, O_TRUNC | O_WRONLY);
		if (fd < 0)
		{
			perror("open");
			printf("fd %s\n", strerror(errno));
			return (-1);
		}
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			printf("%s\n", strerror(errno));
			return (-2);
		}
		redir_list = redir_list->next;
	}
	close(fd);
	return (0);
}
