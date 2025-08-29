/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 07:08:50 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/28 23:15:22 by Seth         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

/*
 * Prat parsing

	https://www.youtube.com/watch?v=0c8b7YfsBKs
*/

static t_tree	*prepare_redir_tree(t_env *minishell, t_list *lst, int *valid, \
	t_token *nud)
{
	t_tree	*nud_t;

	if (!minishell || !lst || !valid)
		return (NULL);
	nud_t = create_word_node_tree(minishell, lst, valid);
	if (!nud_t)
	{
		while (nud)
		{
			if (nud->type != TOK_REDIR_OUT && nud->type != TOK_REDIR_IN && \
				nud->type != TOK_WORD && nud->type != TOK_REDIR_HEREDOC)
				break ;
			nud = consume_token(lst);
		}
		return (prepare(minishell, lst, 0, valid));
	}
	return (nud_t);
}

static int	prepare_tree_while2(t_env *minishell, t_list *lst, \
	t_arg_tree *arg_tree, int *valid)
{
	arg_tree->mid_t = init_node(arg_tree->mid, 1);
	if (!arg_tree->mid_t || !arg_tree->nud_t)
		return (1);
	arg_tree->mid_t->left = arg_tree->nud_t;
	arg_tree->nud_t->top = arg_tree->mid_t;
	arg_tree->tmp = prepare(minishell, lst, arg_tree->power + 1, valid);
	if (!arg_tree->tmp)
		arg_tree->tmp = NULL;
	else
		arg_tree->tmp->top = arg_tree->mid_t;
	arg_tree->mid_t->right = arg_tree->tmp;
	arg_tree->nud_t = arg_tree->mid_t;
	if (arg_tree->mid->type == TOK_PARA_CLOSE)
	{
		consume_token(lst);
		arg_tree->nud_t = prepare(minishell, lst, 0, valid);
	}
	return (0);
}

static void	prepare_tree_while(t_env *minishell, t_list *lst, \
	t_arg_tree *arg_tree, int *valid)
{
	if (!minishell || !lst || !arg_tree || !valid)
		return ;
	while (1)
	{
		arg_tree->mid = see_token(lst);
		if (!arg_tree->mid)
		{
			consume_token(lst);
			break ;
		}
		arg_tree->power = arg_tree->mid->type;
		if (arg_tree->power < arg_tree->min_power)
			break ;
		if (arg_tree->mid->type == TOK_WORD || \
arg_tree->mid->type == TOK_REDIR_IN || arg_tree->mid->type == TOK_WORD || \
	arg_tree->mid->type == TOK_REDIR_HEREDOC)
		{
			consume_token(lst);
			arg_tree->nud_t = create_word_node_tree(minishell, lst, valid);
		}
		else
			arg_tree->mid = consume_token(lst);
		if (prepare_tree_while2(minishell, lst, arg_tree, valid))
			break ;
	}
}

/* Creer un arbre a partir dun liste de token
	https://www.youtube.com/watch?v=0c8b7YfsBKs
*/
t_tree	*prepare(t_env *minishell, t_list *lst, int min_power, int *valid)
{
	t_arg_tree	arg_tree;

	if (!lst)
		return (NULL);
	arg_tree.nud = see_token(lst);
	if (!arg_tree.nud)
		return (NULL);
	else if (arg_tree.nud->type == TOK_REDIR_OUT || \
arg_tree.nud->type == TOK_REDIR_IN || arg_tree.nud->type == TOK_WORD || \
		arg_tree.nud->type == TOK_REDIR_HEREDOC)
		arg_tree.nud_t = prepare_redir_tree(minishell, lst, valid, \
			arg_tree.nud);
	else
	{
		arg_tree.nud = consume_token(lst);
		arg_tree.nud_t = init_node(arg_tree.nud, 1);
	}
	arg_tree.min_power = min_power;
	prepare_tree_while(minishell, lst, &arg_tree, valid);
	return (arg_tree.nud_t);
}
