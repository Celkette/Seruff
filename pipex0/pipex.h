/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:29:11 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/04/01 12:30:29 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "ft_printf/ft_printf.h"
# include "libft/libft.h"
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <errno.h>
# include <string.h>

int		check_cmd(char *cmd);
int		pipex_check_arg(int argc, char **argv);
int		check_if_heredoc(char *argv);
int		pipex_creat_open_fd_in_one(char **argv);
int		pipex_creat_open_fd_out_one(int argc, char **argv);
char	**pipex_ft_split(char const *s, char c);
char	*get_env_path(char **env);
char	**get_env_tab(char **env);
char	*create_full_path(char *path, char *cmd);
char	*get_cmd_path(char *cmd, char **env);
char	**get_cmd_arg(char *cmd);
void	check_file(int argc, char **argv);
void	check_executable(char *cmd, char **env);
void	pipex_error_i(char *message, int i);
void	ft_free_tab(char **tab);
void	begin_check(int argc, char **argv, char **env);
void	execute_cmd(char **argv, char **envp, int cmd_i);

#endif
