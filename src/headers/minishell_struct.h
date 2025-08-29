/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_struct.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 10:11:07 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/27 14:18:45 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_STRUCT_H
# define MINISHELL_STRUCT_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 200
# endif

# include <stdbool.h>
# include <signal.h>
# include "minishell_enum.h"

typedef struct s_file
{
	char	buf[BUFFER_SIZE + 1];
	char	*line;
	int		init;
	int		fd;
	int		status;
	int		index;
}	t_file;

/************************************************************
*  Structure pour contenir les etats des different signaux  *
*		Et une structure sigaction pour leur gestion        *
*************************************************************/
typedef struct s_sig_manage
{
	struct sigaction	s_sig_ctrl_c;
	struct sigaction	s_sig_ctrl_slash;
	bool				sig_ctrl_c;
}	t_sig_manage;

/*****************************************
*  Structure qui stock les info de user *
******************************************/
typedef struct s_user
{
	char			*user;
	char			*home;
	unsigned int	uid;
	unsigned int	gid;
}	t_user;

/**************************
*   Structure du lexing   *
***************************/
typedef struct s_meta
{
	char	last_quote_is;
	int		single_quote;
	int		double_quote;
	int		quote_open;
	int		pipe;
	int		redir_in;
	int		redir_out;
	int		dollar;
	int		space;
	int		only_space;
	int		last_char_pipe;
}	t_meta;

/*************************
*   Structure de liste   *
**************************/
typedef struct s_list
{
	void			*data;
	struct s_list	*next;
	struct s_list	*prev;
	int				del;
}	t_list;

/*************************
*  Structure des tokens  *
**************************/
typedef struct s_token
{
	t_tkty			type;
	char			*data;
	int				consume;
	int				quote_type;
	int				is_quote;
	int				hered;
	int				redir;
}	t_token;

/*********************************
*  Structure des variable pour $ *
**********************************/
//@structure pour les variables
// Contient la clé et la valeur d'une variable
// Utilisée pour stocker les variables locales et d'environnement
// Permet de gérer les variables d'environnement et locales de manière flexible
// Utilisée pour les commandes comme export, unset, et pour l'expansion des 
// 	variables
typedef struct s_var
{
	char	*key;
	char	*val;
}	t_var;

typedef struct s_r_out
{
	int		fd;
	int		type;
	char	*path;
}				t_r_out;

typedef struct s_r_in
{
	int		type;
	int		fd;
	int		is_quote;
	char	*path;
	char	*file;
}				t_r_in;

/*
 * Structure arbre
*/
typedef struct s_tree
{
	char			*content;
	char			*path;
	char			**cmd_list;
	int				type;
	int				status;
	int				stdin_cpy;
	int				stdout_cpy;
	int				here_doc_fd[2];
	int				pipefd_in;
	int				pipefd_out;
	int				(*built_in)(void *);
	int				need_parent;
	pid_t			pid;
	t_list			*arg;
	t_r_in			*redir_in;
	t_list			*redir_out;
	struct s_tree	*top;
	struct s_tree	*left;
	struct s_tree	*right;
}				t_tree;

/*
 * Structure execution
*/
typedef struct s_exec
{
	int	is_pipeline;
	int	pipeline_init;
	int	fd[2];
	int	fd2[2];
	int	exit_code;
	int	exit_code__print;
}				t_exec;

typedef struct s_arg_tree
{
	struct s_tree	*nud_t;
	struct s_tree	*mid_t;
	struct s_tree	*tmp;
	struct s_token	*nud;
	struct s_token	*mid;
	struct s_list	*lst;
	int				power;
	int				min_power;
}				t_arg_tree;

/*
 * Regroupe les autres structure
*/

typedef struct s_manage_readline
{
	int	hd_active;
	int	hd_active_sigint;
	int	in_exec;
	int	abort_exec;
}				t_manage_readline;

typedef struct s_env
{
	struct s_sig_manage	sig;
	struct s_user		user;
	struct s_list		*var;
	struct s_list		*env_list;
	struct s_list		*token_list;
	struct s_list		*pid_list;
	struct s_exec		exec;
	struct s_tree		*tree;
	struct s_meta		*meta;
	struct termios		*term_cpy;
	char				**env;
	char				**path_fun_split;
	char				*last_path;
	char				*act_path;
	char				*last_cmd;
	char				*pid;
	int					found_cmd;
	int					stdin_cpy;
	int					line_amount;
}				t_env;

typedef struct s_arg
{
	t_env	*minishell;
	t_tree	*node;
}				t_arg;

#endif
