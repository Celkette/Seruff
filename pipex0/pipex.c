/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:31:07 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/04/01 12:37:51 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	wait_all_commands_executions(void)
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

static void	first_child(char **argv, char **envp, int *fd)
{
	int	infile;

	infile = pipex_creat_open_fd_in_one(argv);
	close(fd[0]);
	dup2(infile, STDIN_FILENO);
	close(infile);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	execute_cmd(argv, envp, 2);
	pipex_error_i("execute_cmd failed (first_child)", 1);
}

static void	last_child(int argc, char **argv, char **envp, int *fd)
{
	int	outfile;

	outfile = pipex_creat_open_fd_out_one(argc, argv);
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	dup2(outfile, STDOUT_FILENO);
	close(outfile);
	execute_cmd(argv, envp, 3);
	pipex_error_i("execute_cmd failed (second_child)", 1);
}

static void	parent(int argc, char **argv, char **envp, int *fd)
{
	pid_t	pid1;
	pid_t	pid2;

	if (pipe(fd) == -1)
		pipex_error_i("creat pipe failed (in main)", 1);
	pid1 = fork();
	if (pid1 == -1)
		pipex_error_i("fork failed (for first_child)", 1);
	if (pid1 == 0)
		first_child(argv, envp, fd);
	pid2 = fork();
	if (pid2 == -1)
		pipex_error_i("fork failed (for last_child)", 1);
	if (pid2 == 0)
		last_child(argc, argv, envp, fd);
	close(fd[0]);
	close(fd[1]);
	//close(STDIN_FILENO);//-----ON NE FERME PAS LES STANDART
	//close(STDOUT_FILENO);
	//close(STDERR_FILENO);
	wait_all_commands_executions();
}

int	main(int argc, char **argv, char **envp)
{
	int	fd[2];

	if (argc == 5)
	{
		begin_check(argc, argv, envp);
		parent(argc, argv, envp, fd);
	}
	else
		pipex_error_i("\033[31m Mauvais nombre d'arguments\n\033[0m", -1);
	return (0);
}
