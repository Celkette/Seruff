/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_element2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:14:39 by celine            #+#    #+#             */
/*   Updated: 2025/08/19 14:05:26 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

/**********************************
*   FREE pour les listes t_var   *
**********************************/
/************************
*	Libere le NOEUD     * 
*************************/
/******************************************************************************
* Libere le noeud de la liste qui contient la var, (libere var et ses données)
* But: Libère un nœud de liste (t_list *) qui contient un t_var * 
*   dans son champ data.
* Libère :
* 	- var->key & var->val,
* 	- La structure t_var,
* 	- Le nœud de liste lui-même.
* À utiliser : Quand tu veux supprimer un nœud complet de la liste chaînée et 
*   tout ce qu’il contient.
******************************************************************************/
//@ Libere la mémoire allouée pour un noeud de liste contenant une variable 
// @param node Le noeud de liste à libérer
// @utilité: Libère le token contenu dans le noeud, ainsi que la structure
// du noeud lui-même, y compris la chaîne de caractères du token.
void	free_var_node(t_list *node)
{
	t_var	*var;

	if (!node)
		return ;
	var = (t_var *)node->data;
	if (var)
	{
		if (var->key)
			free(var->key);
		if (var->val)
			free(var->val);
		free(var);
	}
	free(node);
}

/***********************************************
*	Libere la liste de t_var, noeud et data   *
************************************************/
//@libere la mémoire allouée pour une liste de t_var
// @param var_list Pointeur vers la liste de t_var à libérer
// @utilité: Parcourt la liste de t_var, libère chaque t_var (et sa data)
void	free_var_list(t_list *var_list)
{
	t_var	*var;
	t_list	*current;
	t_list	*next;

	current = var_list;
	while (current)
	{
		next = current->next;
		var = (t_var *)current->data;
		if (var)
		{
			if (var->key)
				free(var->key);
			if (var->val)
				free(var->val);
			free(var);
		}
		free(current);
		current = next;
	}
}
