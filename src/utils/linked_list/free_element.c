/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_element.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 08:24:08 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/07/27 03:18:00 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

/************************
*	Libere le token     * 
*************************/
/****************************************************************************************
* Libere la structure du token et sa data
* But : Libère une structure t_token * (et sa chaîne de caractères).
* À utiliser : Quand tu as un pointeur direct sur un t_token (pas sur un nœud de liste).
* Ne libère pas le nœud de liste (t_list *) qui pourrait contenir ce token.
*****************************************************************************************/
//@Libère la mémoire allouée pour un token
// @param token Le token à libérer
// @utilité: Libère la structure du token ainsi que sa chaîne de caractères
// (si elle existe).
void	free_token_struct(t_token *token)
{
	if (!token)
		return ;
	free(token->data); // Libère la chaîne de caractères du token
	free(token); // Libère la structure token
}

/************************
*	Libere le NOEUD     * 
*************************/
/****************************************************************************************
* Libere le noeud de liste qui contient le token, le token et sa data
* But: Libère un nœud de liste (t_list *) qui contient un t_token * dans son champ data.
* Libère :
* 	- La chaîne de caractères du token,
* 	- La structure t_token,
* 	- Le nœud de liste lui-même.
* À utiliser : Quand tu veux supprimer un nœud complet de la liste chaînée et tout ce qu’il contient.
*****************************************************************************************/
//@ Libere la mémoire allouée pour un noeud de liste contenant un token 
// @param node Le noeud de liste à libérer
// @utilité: Libère le token contenu dans le noeud, ainsi que la structure
// du noeud lui-même, y compris la chaîne de caractères du token.
void	free_token_node(t_list *node)
{
	t_token	*token;

	if (!node)
		return ;
	token = (t_token *)node->data;
	if (token)
	{
		if (token->data)
			free(token->data);// Libère la chaîne de caractères du token
		free(token);// Libère la structure token
	}
	free(node);// Libère la structure du noeud
}

/***********************************************
*	Libere la liste de tokens, noeud et data   *
************************************************/
//@libere la mémoire allouée pour une liste de tokens
// @param token_list Pointeur vers la liste de tokens à libérer
// @utilité: Parcourt la liste de tokens, libère chaque token (y compris sa chaîne de caractères)
void	free_token_list(t_list *token_list)
{
	t_token	*token;// Pointeur vers struc token(type, *data, quote_type)
	t_list	*current;// Pointeur vers struc list()
	t_list	*next;

	current = token_list;//=tete de liste
	while (current)
	{
		next = current->next;
		token = (t_token *)current->data;
		if (token)
		{
			if (token->data)
				free(token->data);
			free(token);// Libère la structure token
		}
		free(current);// Libère le noeud de la liste
		current = next;
	}
}
