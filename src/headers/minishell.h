/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 09:07:59 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/29 01:22:33 by Seth         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "minishell_enum.h"
# include "minishell_struct.h"
# include "minishell_fun.h"
# include "minishell_messages.h"
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <limits.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <termios.h>
# include <signal.h>
# include <fcntl.h>
# include <curses.h>
# include <term.h>
# include <dirent.h>
# include <errno.h>
# include <features.h>
# include <stdbool.h>//
# include <sys/wait.h>
# include <dirent.h> // Pour le type DIR

extern volatile sig_atomic_t	g_exit_code;
void	fork_heredoc_n(t_env *minishell, t_r_in *redir, t_tree *node);
void	dup_close(int fd, int fd2);
int		manage_error_exit(void *p, t_list *arg);
void	free_redir_in(t_r_in *redir_in);
void	print_export_error(char *s);
int		get_path_env(t_env *minishell);
void	check_exec_cmd(t_env *minishell, t_tree *node);
t_r_in	*do_heredoc(t_r_in *redir_in, t_token *tok, t_env *mini, t_tree *node);
void	reset_term(t_env *minishell);
int		code_converter(int code);
int		check_redir_2(t_list *lst);
void	valid_tree(t_env *minishell, t_tree *tree, int valid);
int		check_cmd(char *cmd);
int		is_a_directory(char *cmd);
void	print_error_d(char *s);
int		open_redir(char *data, int not_append);
int		redir_out_append(char *data, t_list *lst);
int		first_part_cd(void *p, int *code, t_arg *arg, char *new_path);
void	exit_minishell_fork(t_env *minishell);
#endif

//#ifndef COMMON_H
//# define COMMON_H
