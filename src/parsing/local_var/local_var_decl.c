/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   local_var_decl.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 17:06:46 by celine            #+#    #+#             */
/*   Updated: 2025/08/20 10:49:20 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

/***********************************************************
*  Fonction de gestion des declaration de variable locale  *
* si nb_decl == nb_tok alors on gère les variables locales *
* sinon choisi quel token supprimer						   *
***********************************************************/
t_list	*manage_var_decl(t_env *minishell)
{
	int	nb_decl;
	int	nb_tok;

	if (!minishell || !minishell->token_list)
		return (0);
	nb_decl = 0;
	nb_tok = ft_lstsize(minishell->token_list);
	nb_token_var_decl(minishell->token_list, &nb_decl);
	if (nb_decl != 0 && nb_decl == nb_tok)
	{
		manage_update_local_var(minishell, nb_decl);
	}
	if (nb_decl != 0)
	{
		minishell->token_list = choice_if_remove_token(minishell,
				minishell->token_list);
	}
	return (0);
}

/**********************************************************
*  Logique du nombre de declaration de variable           *
*       return (le nombre de declarations)		          *
***********************************************************/
int	nb_token_var_decl(t_list *token_list, int *nb_decl)
{
	t_list	*current;
	t_token	*token;

	*nb_decl = 0;
	if (!token_list)
		return (0);
	current = token_list;
	while (current)
	{
		token = (t_token *)current->data;
		if (check_if_var_decl(token->data) == 1)
		{
			(*nb_decl)++;
		}
		current = current->next;
	}
	return (*nb_decl);
}

/************************************************************
*  Logique de recherche si token = declaration de variable  *
*       return(1) si c'est une declaration                  *
*       return(0) si non                                    *
*************************************************************/
int	check_if_var_decl(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if (str[0] != '_' && ft_isalpha(str[0]) == 0)
	{
		return (0);
	}
	while (str[i] && (str[i] == '_' || ft_isalnum(str[i])))
	{
		i++;
	}
	if (i == 0 || str[i] != '=')
	{
		return (0);
	}
	return (1);
}

/***************************************************
* Logique de recherche si le token prev est export *
*       return(1) si c'est un export sinon (0)     *
****************************************************/
int	is_prev_export(char *prev_node)
{
	if (!prev_node)
		return (0);
	if (prev_node && ft_strncmp(prev_node, "export", 7) == 0)
	{
		return (1);
	}
	return (0);
}
