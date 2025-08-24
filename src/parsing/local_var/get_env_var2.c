/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env_var2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 12:34:31 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/08/21 12:58:11 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

//@Renvoie la valeur d'une variable d'environnement
// @param minishell Pointeur vers la structure d'environnement du shell
// @param var_name Le nom de la variable d'environnement à rechercher
// @return La valeur de la variable d'environnement si trouvée, NULL sinon
t_var	*get_env_var(t_env *minishell, char *var_name)
{
	t_list	*lst;
	t_var	*var;
	size_t	len;

	len = ft_strlen(var_name);
	if (!minishell || !minishell->env_list || !var_name)
		return (0);
	lst = minishell->env_list;
	while (lst)
	{
		var = (t_var *)lst->data;
		if (ft_strncmp(var->key, var_name, len) == 0
			&& ft_strlen(var->key) == ft_strlen(var_name))
			return (var);
		lst = lst->next;
	}
	return (NULL);
}

char	*var_env_value(t_env *minishell, char *var_name)
{
	t_list	*lst;
	t_var	*var;
	size_t	len;

	len = ft_strlen(var_name);
	if (!minishell || !minishell->env_list || !var_name)
		return (0);
	lst = minishell->env_list;
	while (lst)
	{
		var = (t_var *)lst->data;
		if (ft_strncmp(var->key, var_name, len) == 0
			&& ft_strlen(var->key) == ft_strlen(var_name))
			return (var->val);
		lst = lst->next;
	}
	return (NULL);
}

void	safe_add_env_list(t_list *lst, t_var *new_var)
{
	t_list	*tmp;
	t_var	*var;

	if (!lst || !new_var)
		return ;
	tmp = lst;
	while (tmp)
	{
		var = (t_var *)tmp->data;
		if (ft_strncmp(var->key, new_var->key, ft_strlen(var->key)) == 0
			&& ft_strlen(new_var->key) == ft_strlen(var->key))
		{
			if (var->val)
				free(var->val);
			var->val = new_var->val;
			free(new_var->key);
			free(new_var);
			return ;
		}
		tmp = tmp->next;
	}
	ft_lstadd_back(&lst, ft_create_node(new_var));
}
