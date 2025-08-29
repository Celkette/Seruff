/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 08:15:33 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/29 11:02:29 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static t_var	*get_var(t_list *lst, char *key)
{
	if (!lst || !key)
		return (NULL);
	while (lst)
	{
		if (ft_strncmp(((t_var *)lst->data)->key, key, ft_strlen(key) + 1) == 0)
			return (lst->data);
		lst = lst->next;
	}
	return (NULL);
}

static int	want_var_value(t_var *var, char *sep)
{
	int	len;

	len = ft_strlen(sep) - 1;
	if (len < 0)
	{
		var->val = NULL;
		return (0);
	}
	var->val = malloc(sizeof(char) * (len + 1));
	if (!var->val)
		return (-1);
	if (len == 0)
	{
		var->val[0] = 0;  // Chaîne vide pour U=
	}
	else
	{
		ft_memcpy(var->val, sep + 1, len * sizeof(char));
		var->val[len] = 0;
	}
	return (0);
}

t_var	*create_var(char *str, char *sep)
{
	t_var	*var;

	if (!str || !sep || (*str == '_' && *(str + 1) == 0))
		return (NULL);
	var = malloc(sizeof(t_var));
	if (!var)
		return (NULL);
	var->key = malloc(sizeof(char) * (sep - str + 1));
	if (!var->key)
	{
		free(var);
		return (NULL);
	}
	ft_memcpy(var->key, str, (sep - str) * sizeof(char));
	var->key[sep - str] = 0;
	if (want_var_value(var, sep) < 0)
		return (NULL);
	return (var);
}

int	local_var_to_env_var(t_env *minishell, t_tree *node)
{
	t_var	*var;

	if (!minishell || node)
		return (-1);
	var = get_var(minishell->var, (char *)node->arg->data);
	if (!var)
		return (-1);
	ft_lstadd_back(&minishell->env_list, ft_create_node(var));
	return (0);
}

void	print_export_error(char *s)
{
	if (!s)
		return ;
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(s, 2);
	ft_putendl_fd("': not a valid identifier", 2);
}
