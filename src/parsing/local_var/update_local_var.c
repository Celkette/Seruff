/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_local_var.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 17:50:27 by celine            #+#    #+#             */
/*   Updated: 2025/08/20 10:50:02 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

/************************************************************
*  Logique de gestion de mise a jour des declaration        *
*       boucle sur la liste des declaration                 *
*************************************************************/
void	manage_update_local_var(t_env *minishell, int nb)
{
	t_list	*current;
	t_list	*next;
	t_token	*token;
	int		i;

	i = 0;
	current = minishell->token_list;
	while (current && i < nb)
	{
		next = current->next;
		token = (t_token *)current->data;
		update_local_var(minishell, token->data);
		i++;
		current = next;
	}
}

/************************************************************
*  Logique de gestion de mise a jour de la declaration      *
*************************************************************/
//@Ajoute/met a jour une var locale dans la liste des var du minishell
//@param minishell Pointeur vers la structure d'environnement du shell
//@param s Chaine de la variable a ajouter (ex:"VAR=value")
//@return 0 si la variable est valide, -1 si la variable est invalide
// @ note Si la variable est invalide (pas de '=' ou clé vide), rien n'est fait
// @note Si la variable existe deja, sa valeur est mise a jour
// @ note Si la var n'existe pas,struct créee et ajoutée a la liste des var
// @ note Si var est vide(ex:"VAR="),elle est creee+ajoutée avec une valu=vide
void	update_local_var(t_env *minishell, char *s)
{
	char	*key;
	char	*val;
	t_list	*exist_var;
	t_var	*new_var;

	if (!minishell || !s)
		return ;
	if (parse_var(s, &key, &val) != 0)
	{
		return ;
	}
	exist_var = is_var_in_local(minishell, key);
	if (exist_var)
		minishell->var = ft_delete_node_var(exist_var);
	new_var = creat_local_var(key, val);
	if (new_var)
		minishell->var = ft_add_node(minishell->var, new_var);
	free(key);
	free(val);
}
