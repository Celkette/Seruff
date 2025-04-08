/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:55:47 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/04/01 14:00:13 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static int	manage_first_child(char **argv, char **envp, int *pipe_in)
{
	int		fd[2];
	pid_t	pid;

	if (pipe(fd) == -1)
		pipex_error_i("La création du pipe a échoué (premier enfant)", -1);
	pid = fork();
	if (pid == -1)
		pipex_error_i("La création du premier enfant a échoué", -1);
	if (pid == 0)
		first_child_bonus(argv, envp, fd);
	close(fd[1]);
	*pipe_in = fd[0];
	return (0);
}

static int	manage_middle_child(char **argv, char **envp, int i, int *pipe_in)
{
	int		fd[2];
	pid_t	pid;
	int		i_fd[3];

	i_fd[0] = i;
	if (pipe(fd) == -1)
		pipex_error_i("La création du pipe a échoué (middle child)", -1);
	pid = fork();
	if (pid == -1)
		pipex_error_i("La création d'un enfant du milieu a échoué", -1);
	if (pid == 0)
	{
		i_fd[1] = fd[0];
		i_fd[2] = fd[1];
		middle_child_bonus(argv, envp, i_fd, *pipe_in);
	}
	close(fd[1]);
	close(*pipe_in);
	*pipe_in = fd[0];
	return (0);
}

static int	manage_last_child(int argc, char **argv, char **envp, int *pipe_in)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		pipex_error_i("La création du dernier enfant a échoué", -1);
	if (pid == 0)
		last_child_bonus(argc, argv, envp, *pipe_in);
	close(*pipe_in);
	return (0);
}

static void	parent_bonus(int argc, char **argv, char **envp)
{
	int	i;
	int	pipe_in;
	int	cmd_start_i;
	int	child_nb;

	pipe_in = -1;
	cmd_start_i = 2;
	if (check_if_heredoc(argv[1]) == 0)
		cmd_start_i = 3;
	child_nb = argc - 1 - cmd_start_i;
	i = cmd_start_i;
	manage_first_child(argv, envp, &pipe_in);
	i++;
	child_nb++;
	while (i < argc - 2)
	{
		manage_middle_child(argv, envp, i, &pipe_in);
		i++;
		child_nb++;
	}
	manage_last_child(argc, argv, envp, &pipe_in);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	wait_all_commands_executions();
}

int	main(int argc, char **argv, char **envp)
{
	if (argc >= 5)
	{
		begin_check(argc, argv, envp);
		parent_bonus(argc, argv, envp);
	}
	else
		pipex_error_i("\033[31m Mauvais nombre d'arguments\n\e[0m", -1);
	return (0);
}
