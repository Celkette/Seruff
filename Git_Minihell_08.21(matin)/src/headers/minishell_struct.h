/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_struct.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 10:11:07 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/17 07:38:47 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_STRUCT_H
# define MINISHELL_STRUCT_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 200
# endif

#include <stdbool.h>
#include <signal.h>
#include "minishell_enum.h"

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
	int		single_quote;
	int		double_quote;
	int 	quote_open;// Indique si une quote est ouverte
	char	last_quote_is;// Indique le type de quote ouverte
	int		pipe;
	// Pour '<' et '<<'
	int		redir_in;
	// Pour '>' et '>>'
	int		redir_out;
	int		dollar;
	int		space;
	int		only_space;
	int		last_char_pipe;
	// (esperluette) Pour '&'
	int		ampersand;
	int		last_ampersand;
	int		parenthesis_in;
	int		parenthesis_out;
	int		found_cmd;
}	t_meta;

/*************************
*   Structure de liste   *
**************************/
typedef struct s_list
{
	void			*data;// == token
	struct s_list	*next;
	struct s_list	*prev;
	int	del;//noeud supprimé 1 =suivant -1 = precedent
	//struct s_list	*prev;//seulement si on veut une liste doublement chainée
	//voir si on garde dans la structure ou si on cree variable si besoin
	//struct s_list	*head;//tous les noeuds pointeront sur la tete de liste
}	t_list;

/*************************
*  Structure des tokens  *
**************************/
typedef struct s_token
{
	t_token_type	type;// Type du token
	char			*data;// == chaine str
//	int 			ndollar;//presence d'un ou plusieur dollar
//	char 			dollar_data;// Pour stocker la valeur de la variable 
	int				consume;
	int				quote_type;      //quote 0=aucune, 1=simples, 2=doubles
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
// Permet de gérer les variables d'environnement et locales de manière flexible.
// Utilisée pour les commandes comme export, unset, et pour l'expansion des variables
typedef struct s_var
{								// ex: PATH=/usr/bin:/bin
	char	*key;				// key="PATH"
	char	*val;				// val="/usr/bin:/bin" 
}	t_var;

typedef struct	s_redir_out
{
	int		fd;
	int		type;
	char	*path;
}				t_redir_out;

typedef struct	s_redir_in
{
	int		type;
	char	*path;
}				t_redir_in;
/*
 * Structure arbre
*/

typedef struct	s_tree
{
	char			*content;
	char			*path;
	char			**cmd_list;
	int				type;
	int				status;
	int				here_doc_fd[2];
	int				pipefd_in;
	int				pipefd_out;
	int				(*built_in)(void *);
	int				need_parent;
	t_list			*arg;
	t_redir_in		*redir_in;
	t_list			*redir_out;
	struct s_tree	*top;
	struct s_tree	*left;
	struct s_tree	*right;
}				t_tree;


/*
 * Structure execution
*/
typedef struct	s_exec
{
	int	is_pipeline;
	int	pipeline_init;
	int	fd[2];
	int	fd2[2];
	int	exit_code;
}				t_exec;

typedef struct	s_arg_tree
{
	struct s_tree	*nud_t;
	struct s_tree	*mid_t;
	struct s_tree	*tmp;
	struct s_token	*nud;
	struct s_token	*mid;
	struct s_list	*lst;
	int				power;
	int				min_power;
} t_arg_tree;

/*
 * Regroupe les autres structure
*/

typedef struct	s_manage_readline
{
	int	hd_active;
	int	hd_active_sigint;
	int	in_exec;
	int	abort_exec;
} t_manage_readline;

typedef struct s_env
{
	struct s_sig_manage	sig;
	struct s_user		user;
	struct s_list		*var;
	struct s_list		*env_list;
	struct s_list		*token_list;
	struct s_exec		exec;
	struct s_tree		*tree;
	struct termios		*term_cpy;
	char				**env;
	char				**path_fun_split;
	char				*last_path;
	char				*act_path;
	char				*last_cmd;
	char				*pid;
	int					found_cmd;
}	t_env;

typedef struct	s_arg
{
	t_env	*minishell;
	t_tree	*node;
}	t_arg;

#endif
