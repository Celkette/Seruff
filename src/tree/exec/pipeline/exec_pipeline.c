/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaporte <nlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 19:09:06 by nlaporte          #+#    #+#             */
/*   Updated: 2025/07/29 04:50:07 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

// gestion erreur dup2
static void	fork_dup_fd(int old, int new)
{
	if (new < 0)
		return ;
	if (dup2(old, new) < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putendl_fd(strerror(errno), 2);
		ft_putstr_fd("old = ", 2);
		ft_putendl_fd(ft_itoa(old), 2);
		ft_putstr_fd("new = ", 2);
		ft_putendl_fd(ft_itoa(new), 2);
		perror("dup2");
		exit(1);
	}
}

// duplique les bons fd en fonctions de lemplacement de la commande
static void	fork_dup(t_env *minishell, t_tree *node, int last, int i)
{
	if (i == 0 && !node->redir_in)
		fork_dup_fd(minishell->exec.fd[1], STDOUT_FILENO);
	else if (i % 2)
	{
		if (last && !node->redir_in)
			fork_dup_fd(minishell->exec.fd[0], STDIN_FILENO);
		else
		{
			if (!node->redir_in)
				fork_dup_fd(minishell->exec.fd[0], STDIN_FILENO);
			if (!node->redir_out)
				fork_dup_fd(minishell->exec.fd2[1], STDOUT_FILENO);
		}
	}
	else
	{
		if (last && !node->redir_in)
			fork_dup_fd(minishell->exec.fd2[0], STDIN_FILENO);
		else
		{
			if (!node->redir_in)
				fork_dup_fd(minishell->exec.fd2[0], STDIN_FILENO);
			if (!node->redir_out)
				fork_dup_fd(minishell->exec.fd[1], STDOUT_FILENO);
		}
	}
}

// Dup les bons fd, close les fd, puis exec la commande
static void	fork_part(t_env *minishell, t_tree *node, int last, int i)
{
	fork_dup(minishell, node, last, i);
	if (minishell->exec.fd[0] != -1)
	{
		close(minishell->exec.fd[0]);
		minishell->exec.fd[0] = -1;
	}
	if (minishell->exec.fd[1] != -1)
	{
		close(minishell->exec.fd[1]);
		minishell->exec.fd[1] = -1;
	}
	if (minishell->exec.fd2[0] != -1)
	{
		close(minishell->exec.fd2[0]);
		minishell->exec.fd2[0] = -1;
	}
	if (minishell->exec.fd2[1] != -1)
	{
		close(minishell->exec.fd2[1]);
		minishell->exec.fd2[1] = -1;
	}
	exec_cmd(minishell, node);
}

static void	close_pipe(t_env *minishell, int last, int i)
{
	(void)last;
	if (i % 2)
	{
		fprintf(stderr, "fd[0] %i et fd2[1] %i\n", minishell->exec.fd[0], minishell->exec.fd2[1]);
		if (minishell->exec.fd[0] != -1)
		{
			close(minishell->exec.fd[0]);
			minishell->exec.fd[0] = -1;
		}
		if (minishell->exec.fd2[1] != -1)
		{
			close(minishell->exec.fd2[1]);
			minishell->exec.fd2[1] = -1;
		}
	}
	else
	{
		fprintf(stderr, "fd2[0] %i et fd[1] %i\n", minishell->exec.fd2[0], minishell->exec.fd[1]);
		if (minishell->exec.fd2[0] != -1)
		{
			close(minishell->exec.fd2[0]);
			minishell->exec.fd2[0] = -1;
		}
		if (minishell->exec.fd[1] != -1)
		{
			close(minishell->exec.fd[1]);
			minishell->exec.fd[1] = -1;
		}
	}
}

// Creer les pipe en fonction de lemplacement des commandes
static void	init_pipe(t_env *minishell, int last, int i)
{
	ft_putstr_fd("mdr = ", 2);
	ft_putendl_fd(ft_itoa(last), 2);
	if (!minishell)
		return ;
	if (i % 2)
	{
		if (minishell->exec.fd2[0] != -1)
			close(minishell->exec.fd2[0]);
		if (minishell->exec.fd2[1] != -1)
			close(minishell->exec.fd2[1]);
		ft_putendl_fd("Je pipe la i % 2", 2);
		if (pipe(minishell->exec.fd2) >= 0)
			return ;
	}
	else
	{
		if (minishell->exec.fd[0] != -1)
			close(minishell->exec.fd[0]);
		if (minishell->exec.fd[1] != -1)
			close(minishell->exec.fd[1]);
		ft_putendl_fd("Je pipe la i % 2", 2);
		if (pipe(minishell->exec.fd) >= 0)
			return ;
	}
	ft_putstr_fd("can't pipe: ", 2);
	ft_putendl_fd(strerror(errno), 2);
	exit(1);
}

// Prepare la pipeline puis, fork la commande a executer et ferme les fds des pipes
void	exec_pipeline(t_env *minishell, t_tree *node, int last, int i)
{
	pid_t pid;

	if (!minishell || !node || node->status == 1)
		return ;
	init_pipe(minishell, last, i);
	node->status = 1;
	pid = fork();
	if (pid < 0)
	{
		ft_putstr_fd("fork error\n", 2);
		return ;
	}
	else if (pid == 0)
		fork_part(minishell, node, last, i);
	else
		close_pipe(minishell, last, i);
}
