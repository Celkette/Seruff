/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   local_var_decl2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 10:34:54 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/08/20 11:12:06 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

/*****************************************************************
*  Logique de remplacement des tokens de declaration de variable  *
*  par une commande qui ne fait rien == true  					  *
******************************************************************/
static void	replace_var_decl_tokens(t_env *minishell, t_list *token_list)
{
	t_list	*current;
	t_list	*next;
	t_token	*token;

	minishell->found_cmd = 0;
	current = token_list;
	while (current)
	{
		next = current->next;
		token = (t_token *)current->data;
		if (!minishell->found_cmd)
		{
			if (token->type == TOK_WORD && check_if_var_decl(token->data))
			{
				free(token->data);
				token->data = strdup("true");
			}
			else if (token->type == TOK_WORD && !check_if_var_decl(token->data))
				minishell->found_cmd = 1;
		}
		if (token->type == TOK_PIPE)
			minishell->found_cmd = 0;
		current = next;
	}
}

t_list	*choice_if_remove_token(t_env *minishell, t_list *token_list)
{
	if (!token_list)
		return (NULL);
	replace_var_decl_tokens(minishell, token_list);
	token_list = head_of_list(token_list);
	return (token_list);
}

/*
t_list	*choice_if_remove_token(t_env *minishell, t_list *token_list)
{
	t_list	*current;
	t_list	*next;
	t_token	*token;

	if (!token_list)
		return (NULL);
	minishell->found_cmd = 0;
	current = token_list;
	while (current)
	{
		next = current->next;
		token = (t_token *)current->data;
		if (!minishell->found_cmd)
		{
			if (token->type == TOK_WORD && check_if_var_decl(token->data))
			{
				//token_list = ft_delete_remove_token(current);
				free(token->data);
				token->data = strdup("true");
			}
			else if (token->type == TOK_WORD && !check_if_var_decl(token->data))
				minishell->found_cmd = 1;
		}
		if (token->type == TOK_PIPE)
				minishell->found_cmd = 0;
		current = next;
	}
	token_list = head_of_list(token_list);
	return (token_list);
}*/
