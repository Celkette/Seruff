/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_stdin2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 11:32:50 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/08/21 11:11:10 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/minishell.h"
#include <readline/readline.h>

//@ Boucle principale du shell
//@lit l’entrée utilisateur, traite les commandes 
//spéciales, affiche le split, puis lance le lexing.
//(Utilité : Cœur de la boucle d’interprétation du shell.)
// @param minishell Pointeur vers la structure d'environnement du shell
static int	read_stdin2(t_env *minishell, char *str, t_meta *meta)
{
	int	code_syntaxe;

	if (!str || !minishell)
	{
		rl_clear_history();
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "exit\n", 5);
		return (exit_minishell(minishell), -1);
	}
	else
	{
		if (*str)
			add_history(str);
		code_syntaxe = check_lexing(str, meta);
		if (code_syntaxe == 0)
			try_tokenize_hell(minishell, str, meta);
		else
		{
			minishell->exec.exit_code = code_syntaxe;
			g_exit_code = 0;
		}
		free(str);
		str = NULL;
	}
	return (0);
}

static void	read_stdin3(char *str)
{
	if (str)
		free(str);
}

void	clear_pid_list(t_env *minishell)
{
	t_list	*lst;
	t_list	*tmp;

	if (!minishell)
		return ;
	lst = minishell->pid_list;
	while (lst)
	{
		tmp = lst;
		lst = lst->next;
		free(tmp->data);
		free(tmp);
	}
}

char	*read_stdin_init_while(t_env *minishell, char *str)
{
	if (!minishell)
		return (NULL);
	signal(SIGINT, sig_ctrl_c_catcher);
	str = readline("minishell-1.0$ ");
	if (minishell->line_amount >= INT_MAX - 2)
		minishell->line_amount = 0;
	minishell->line_amount++;
	signal(SIGINT, sig_ctrl_c_catcher2);
	return (str);
}

void	read_stdin(t_env *minishell, t_meta *meta, char *str, char *tmp)
{
	minishell->stdin_cpy = dup(STDIN_FILENO);
	str = NULL;
	while (1)
	{
		if (isatty(STDIN_FILENO))
			str = read_stdin_init_while(minishell, str);
		else
		{
			str = get_next_line(STDIN_FILENO);
			if (!str)
				break ;
			tmp = ft_strndup(str, ft_strlen(str) - 1);
			if (!tmp)
				break ;
			(free(str), str = tmp);
		}
		if (read_stdin2(minishell, str, meta) == -1)
			break ;
		dup2(minishell->stdin_cpy, STDIN_FILENO);
	}
	read_stdin3(str);
	if (!isatty(STDIN_FILENO))
		exit_minishell(minishell);
}
