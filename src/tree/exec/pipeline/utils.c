/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 18:05:58 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/19 13:49:50 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

// gestion erreur dup2
void	fork_dup_fd(t_env *minishell, t_tree *node, int old, int neew)
{
	if (neew < 0 || !node || (!node->path && !node->built_in))
	{
		if (minishell->exec.fd[0] > 0)
			close(minishell->exec.fd[0]);
		if (minishell->exec.fd[1] > 0)
			close(minishell->exec.fd[1]);
		if (minishell->exec.fd2[0] > 0)
			close(minishell->exec.fd[0]);
		if (minishell->exec.fd2[1] > 0)
			close(minishell->exec.fd[1]);
		exit_minishell(minishell);
		exit(127);
	}
	if (dup2(old, neew) < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putendl_fd(strerror(errno), 2);
		exit(24);
	}
}

// duplique les bons fd en fonctions de lemplacement de la commande
void	fork_dup(t_env *minishell, t_tree *node, int last, int i)
{
	if (i == 0 && !node->redir_in)
		fork_dup_fd(minishell, node, minishell->exec.fd[1], STDOUT_FILENO);
	else if (i % 2)
	{
		if (last && !node->redir_in)
			return ((void)fork_dup_fd(minishell, node, minishell->exec.fd[0], \
			STDIN_FILENO));
		if (!node->redir_in && minishell->exec.fd[0] >= 0)
			fork_dup_fd(minishell, node, minishell->exec.fd[0], STDIN_FILENO);
		if (!node->redir_out && !last && minishell->exec.fd2[0] >= 0)
			fork_dup_fd(minishell, node, minishell->exec.fd2[1], STDOUT_FILENO);
	}
	else
	{
		if (last && !node->redir_in)
			return ((void)fork_dup_fd(minishell, node, minishell->exec.fd2[0], \
			STDIN_FILENO));
		if (!node->redir_in && minishell->exec.fd2[0] >= 0)
			fork_dup_fd(minishell, node, minishell->exec.fd2[0], STDIN_FILENO);
		if (!node->redir_out && !last && minishell->exec.fd[1] >= 0)
			fork_dup_fd(minishell, node, minishell->exec.fd[1], STDOUT_FILENO);
	}
}

/*
/ duplique les bons fd en fonctions de lemplacement de la commande
void	fork_dup(t_env *minishell, t_tree *node, int last, int i)
{
	if (i == 0 && !node->redir_in)
		fork_dup_fd(minishell, node, minishell->exec.fd[1], STDOUT_FILENO);
	else if (i % 2)
	{
		if (last && !node->redir_in)
			return ((void)fork_dup_fd(minishell, node, minishell->exec.fd[0], \
			STDIN_FILENO));
		if (!node->redir_in && minishell->exec.fd[0] >= 0)
			fork_dup_fd(minishell, node, minishell->exec.fd[0], STDIN_FILENO);
		if (!node->redir_out && !last && minishell->exec.fd2[0] >= 0)
			fork_dup_fd(minishell, node, minishell->exec.fd2[1], STDOUT_FILENO);
	}
	else
	{
		if (last && !node->redir_in)
			return ((void)fork_dup_fd(minishell, node, minishell->exec.fd2[0], \
			STDIN_FILENO));
		if (!node->redir_in && minishell->exec.fd2[0] >= 0)
			fork_dup_fd(minishell, node, minishell->exec.fd2[0], STDIN_FILENO);
		if (!node->redir_out && !last && minishell->exec.fd[1] >= 0)
			fork_dup_fd(minishell, node, minishell->exec.fd[1], STDOUT_FILENO);
	}
}
*/

void	set_fd_in_node(t_env *minishell, t_tree *node, int last, int i)
{
	if (!minishell || !node)
		return ;
	if (!last && i % 2)
		node->pipefd_out = minishell->exec.fd2[1];
	else if (!last)
		node->pipefd_out = minishell->exec.fd[1];
	if (i <= 0)
		return ;
	if (i % 2)
		node->pipefd_in = minishell->exec.fd[0];
	else
		node->pipefd_in = minishell->exec.fd2[0];
}

// Dup les bons fd, close les fd, puis exec la commande
void	fork_part(t_env *minishell, t_tree *node, int last, int i)
{
	fork_dup(minishell, node, last, i);
	if (minishell->exec.fd[0] > 0)
	{
		close(minishell->exec.fd[0]);
		minishell->exec.fd[0] = -1;
	}
	if (minishell->exec.fd[1] > 0)
	{
		close(minishell->exec.fd[1]);
		minishell->exec.fd[1] = -1;
	}
	if (minishell->exec.fd2[0] > 0)
	{
		close(minishell->exec.fd2[0]);
		minishell->exec.fd2[0] = -1;
	}
	if (minishell->exec.fd2[1] > 0)
	{
		close(minishell->exec.fd2[1]);
		minishell->exec.fd2[1] = -1;
	}
	close(minishell->stdin_cpy);
	exec_cmd(minishell, node, 0);
}
