/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:23:45 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/04/01 15:49:08 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "pipex.h"
# include "ft_printf/ft_printf.h"
# include "libft/libft.h"
# include "gnl/get_next_line.h"
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <errno.h>
# include <string.h>

int		open_file(char *file, int flag);
int		pipex_creat_here_doc(char *limiter);
int		pipex_creat_open_fd_in(char **argv);
int		pipex_creat_open_fd_out(int argc, char **argv);
void	first_child_bonus(char **argv, char **envp, int *fd);
void	middle_child_bonus(char **argv, char **envp, int *cmd_i, int pipe_in);
void	last_child_bonus(int argc, char **argv, char **envp, int pipe_in);
void	wait_all_commands_executions(void);

#endif
