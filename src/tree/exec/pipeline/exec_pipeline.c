/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 19:09:06 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/21 13:55:25 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

static void	close_pipe(t_env *minishell, int i, int last)
{
	if (i % 2 && minishell)
	{
		if (minishell->exec.fd[0] != -1)
			close(minishell->exec.fd[0]);
		minishell->exec.fd[0] = -1;
		if (minishell->exec.fd2[1] != -1)
			close(minishell->exec.fd2[1]);
		minishell->exec.fd2[1] = -1;
	}
	else if (minishell)
	{
		if (minishell->exec.fd2[0] != -1)
			close(minishell->exec.fd2[0]);
		minishell->exec.fd2[0] = -1;
		if (minishell->exec.fd[1] != -1)
			close(minishell->exec.fd[1]);
		minishell->exec.fd[1] = -1;
	}
	if (last && minishell)
		close_pipe(minishell, i + 1, 0);
}

// Creer les pipe en fonction de lemplacement des commandes
static int	init_pipe(t_env *minishell, int i, int last)
{
	if (!minishell)
		return (0);
	if (last)
		return (1);
	if (i % 2)
	{
		if (minishell->exec.fd2[0] != -1)
			close(minishell->exec.fd2[0]);
		if (minishell->exec.fd2[1] != -1)
			close(minishell->exec.fd2[1]);
		if (pipe(minishell->exec.fd2) >= 0)
			return (1);
	}
	else
	{
		if (minishell->exec.fd[0] != -1)
			close(minishell->exec.fd[0]);
		if (minishell->exec.fd[1] != -1)
			close(minishell->exec.fd[1]);
		if (pipe(minishell->exec.fd) >= 0)
			return (1);
	}
	ft_putstr_fd("minishell: ", 2);
	ft_putendl_fd(strerror(errno), 2);
	return (1);
}

// Prepare la pipeline puis, fork la commande 
// a executer et ferme les fds des pipes
int	exec_pipeline(t_env *minishell, t_tree *node, int last, int i)
{
	pid_t	pid;

	if (!minishell || !node || node->status == 1)
		return (127);
	if (!init_pipe(minishell, i, last))
	{
		close_pipe(minishell, 0, 1);
		return (-1);
	}
	node->status = 1;
	if (i == 0)
		update_last_cmd(minishell, ft_strndup(node->content,
				ft_strlen(node->content)));
	pid = fork();
	if (pid < 0)
	{
		ft_putstr_fd("fork error\n", 2);
		return (-1);
	}
	else if (pid == 0)
		fork_part(minishell, node, last, i);
	else
		close_pipe(minishell, i, last);
	return (0);
}
