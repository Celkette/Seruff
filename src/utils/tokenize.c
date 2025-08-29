/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 12:06:04 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/08/29 12:06:21 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "src/headers/minishell.h"

void	tokenize_expand_quote(t_env *minishell, char *s, t_meta *meta)
{
	free_token_list(minishell->token_list);
	minishell->token_list = tokenize_the_hell(s, "< >|	");
	minishell->token_list = expand_the_list(minishell,
			minishell->token_list, meta);
	manage_var_decl(minishell);
	minishell->token_list = remove_list_quotes(minishell->token_list);
}

//@Découpe la ligne de commande en tokens 
//selon les séparateurs donnés et affiche chaque token
//(Utilité : Tester et visualiser le résultat du split (tokenisation))
// @param s La chaîne de caractères à découper
void	try_tokenize_hell(t_env *minishell, char *s, t_meta *meta)
{
	int		valid;
	int		code;

	tokenize_expand_quote(minishell, s, meta);
	if (!minishell->token_list)
		return ;
	code = check_redir(minishell->token_list);
	if (check_redir_2(minishell->token_list) != 0)
	{
		valid = -1;
		return ;
	}
	if (code)
	{
		minishell->exec.exit_code = 2;
		g_exit_code = 0;
	}
	if (code == 2)
	{
		valid = -1;
		return ;
	}
	valid = 1;
	minishell->tree = prepare(minishell, minishell->token_list, 0, &valid);
	try_tokenize_hell2(minishell, valid);
}

void	try_tokenize_hell2(t_env *minishell, int valid)
{
	t_tree	*tree;

	if (minishell && minishell->tree && valid)
	{
		tree = minishell->tree;
		valid_tree(minishell, tree, valid);
		free_tree(minishell->tree);
		minishell->tree = NULL;
	}
}
