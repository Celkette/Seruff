/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   local_var_list.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 07:36:03 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/08/17 12:50:21 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

/************************************************************
*      void check_local_var(t_env *minishell, char *s)      *
* 1. parse s en key et val (malloc par parse_var())         *
* 2. cherche si la variable existe deja dans minishell->var *
*       is_var_in_local()                                    *
* 3. si oui, supprime l'ancienne variable de la liste       *
*        ft_delete_node()                                   *
* 4. cree une nouvelle variable locale                      *
*        creat_local_var()                                  *
* 5. ajoute la nouvelle variable a la liste                 *
*        ft_add_node()                                      *
* 6. free key et val (malloc par parse_var)                 *
*************************************************************/
/***********************************************************
 * La variable est elle dans la liste des variable locale? *
 ***********************************************************/
//@Cherhe la var var_name dans la liste des var locales(minishell->var)
// @param list Pointeur vers la liste des variables locales
// @param s Chaine de la variable a rechercher (ex:"VAR")
// @param var Pointeur vers la structure t_var a remplir avec la valeur trouvee
// @return Pointeur vers la structure de la variable si trouvee, NULL sinon
//int	is_var_in_local(t_list *list, char *var_name)
t_list	*is_var_in_local(t_env *minishell, char *var_name)
{
	t_list	*current;
	t_var	*var_str;

	if (!minishell->var || !var_name)
	{
		return (0);
	}
	current = minishell->var;
	while (current)
	{
		var_str = (t_var *)current->data;
		if (var_str && var_str->key
			&& ft_strlen(var_name) == ft_strlen(var_str->key)
			&& ft_strncmp(var_name, var_str->key, ft_strlen(var_name)) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

/*********************************************************
 * 	Decoupe la nouvelle variable locale NAME=celine	     *
 * 		pour la stocker dans une structure t_var 		 *
 * 			var->key=NAME et var->val=celine			 *
 *********************************************************/
//@parse la string en *key et *val qui seront alloués dynamiquement
// @param s Chaine de la variable a ajouter (ex:"VAR=value")
// @param key Pointeur vers la clé de la variable (ex:"VAR")
// @param val Pointeur vers la valeur de la variable (ex:"value")
// @return 0 si la variable est valide, -1 si la variable est invalide
// @note Si la variable est invalide (pas de '=' ou clé vide), retourne -1	
int	parse_var(char *s, char **key, char **val)
{
	int		key_len;
	int		val_len;

	key_len = 0;
	val_len = 0;
	if (!s || !key || !val)
		return (-1);
	if (!ft_strchr(s, '=') || s[0] == '=')
		return (-1);
	while (s[key_len] && s[key_len] != '=')
		key_len++;
	while (s[val_len + key_len + 1])
		val_len++;
	*key = ft_strndup(s, key_len);
	*val = ft_strndup((s + key_len + 1), val_len);
	if (!*val || !*key)
	{
		free(*key);
		free(*val);
		return (-1);
	}
	return (0);
}

/***********************************************************
 * Crée la structure t_var de la nouvelle variable locale  *
 * 			qui sera ajoutée dans la liste chainée	       *
 ***********************************************************/
//@Cree pour var locale une stuct t_var et initialise ses champs key et val
// @param key Clé de la variable (ex:"NAME)
// @param val Valeur de la variable (ex:"celine")
// @return Pointeur vers la struc t_var si l'alloc réussit, NULL sinon
t_var	*creat_local_var(char *key, char *val)
{
	t_var	*var;

	if (!key || !val)
		return (NULL);
	var = malloc(sizeof(t_var));
	if (!var)
		return (NULL);
	var->key = ft_strdup(key);
	var->val = ft_strdup(val);
	return (var);
}

/***************************************************************************
 * Libere mémoire allouée pour chaque var (struct, clé et valeur) et noeud *
 * 	util pour -À la fermeture du shell (obligatoire)	       	           *
 *			  -En cas d'erreur fatale (cleanup)	       	     			   *
 *			  -Dans les tests (pour valgrind)	       	            	   *
 *            -Reset complet des variables (si besoin)	       	           *
 ***************************************************************************/
//@Libere mémoire allouée pour list des var locales avant de quitter le shell
// @param minishell Pointeur vers la structure d'environnement du shell
// Cette fonction parcourt la liste des var d'env, libère la mémoire
// allouée pour chaque var(clé et valeur), ainsi que pour le noeud de la liste.
void	clean_var(t_env *minishell)
{
	t_list	*to_free;
	t_var	*var;

	if (!minishell || !minishell->var)
		return ;
	while (minishell->var)
	{
		var = (t_var *)minishell->var->data;
		free(var);
		to_free = minishell->var;
		minishell->var = minishell->var->next;
		free(to_free);
	}
}

/************************************************
* Renvoie la valeur de la var locale si trouvée * 
*************************************************/
char	*var_local_value(t_env *minishell, char *var_name)
{
	t_list	*current;
	t_var	*var_str;
	int		len;

	if (!minishell->var || !var_name)
		return (NULL);
	len = ft_strlen(var_name);
	current = minishell->var;
	while (current)
	{
		var_str = (t_var *)current->data;
		if (var_str && var_str->key && len == ft_strlen(var_str->key)
			&& ft_strncmp(var_name, var_str->key, len) == 0)
			return (var_str->val);
		current = current->next;
	}
	return (NULL);
}
