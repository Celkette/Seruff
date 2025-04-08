/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus_utils_1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:54:39 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/04/01 15:22:04 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	wait_all_commands_executions(void)
{
	pid_t	wpid;
	int		status;

	wpid = 1;
	status = 0;
	while (wpid > 0)
	{
		wpid = wait(&status);
	}
}

void	first_child_bonus(char **argv, char **envp, int *fd)
{
	int	fd_in;
	int	cmd_arg ;

	cmd_arg = 2;
	if (check_if_heredoc(argv[1]) == 0)
		cmd_arg = 3;
	fd_in = pipex_creat_open_fd_in(argv);
	close(fd[0]);
	if (dup2(fd_in, STDIN_FILENO) == -1)
		pipex_error_i("dup2 failed (first_child_bonus)", 1);
	close(fd_in);
	if (dup2(fd[1], STDOUT_FILENO) == -1)
		pipex_error_i("dup2 failed (first_child_bonus)", 1);
	close(fd[1]);
	execute_cmd(argv, envp, cmd_arg);
	pipex_error_i("execute_cmd failed (first_child_bonus)", 1);
}

void	middle_child_bonus(char **argv, char **envp, int *cmd_i, int pipe_in)
{
	close(cmd_i[1]);
	if (dup2(pipe_in, STDIN_FILENO) == -1)
		pipex_error_i("dup2 failed (middle_child_bonus)", 1);
	close(pipe_in);
	if (dup2(cmd_i[2], STDOUT_FILENO) == -1)
		pipex_error_i("dup2 failed (middle_child_bonus)", 1);
	close(cmd_i[2]);
	execute_cmd(argv, envp, cmd_i[0]);
	pipex_error_i("execute_cmd failed (middle_child_bonus)", 1);
}

// Fonction pour le dernier enfant dans la version bonus
void	last_child_bonus(int argc, char **argv, char **envp, int pipe_in)
{
	int	fd_out;
	int	cmd_arg;

	cmd_arg = argc - 2;
	fd_out = pipex_creat_open_fd_out(argc, argv);
	if (dup2(pipe_in, STDIN_FILENO) == -1)
		pipex_error_i("dup2 failed (last_child_bonus)", 1);
	close(pipe_in);
	if (dup2(fd_out, STDOUT_FILENO) == -1)
		pipex_error_i("dup2 failed (last_child_bonus)", 1);
	close(fd_out);
	execute_cmd(argv, envp, cmd_arg);
	pipex_error_i("execute_cmd failed (last_child_bonus)", 1);
}
