/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaporte <nlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 18:19:56 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/13 18:27:20 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

int	print_error_pipeline(char *s, char *err, int code)
{
	ft_putstr_fd(s, 2);
	ft_putendl_fd(err, 2);
	return (code);
}

int	use_redir_in_here_doc2(t_tree *node, pid_t fork_pid, char *path_p)
{
	int	code;

	if (!node)
		return (-1);
	if (fork_pid == 0)
	{
		close(node->here_doc_fd[0]);
		code = write(node->here_doc_fd[1], path_p, 1000);
		while (code == 1000)
		{
			path_p += 1000;
			code = write(node->here_doc_fd[1], path_p, 1000);
		}
		if (code < 0)
			return (print_error_pipeline("can't write: %s", \
				strerror(errno), -4));
		close(node->here_doc_fd[1]);
		exit(0);
	}
	return (0);
}
