/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:27:16 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/21 11:20:19 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

int	export_built_in(void *p)
{
	t_arg	*arg;
	char	*tmp;
	char	*sep;
	int		code;

	if (!p)
		return (-1);
	code = 0;
	arg = (t_arg *)p;
	if (!arg->node || !arg->minishell)
	{
		free(p);
		return (-1);
	}
	if (!arg->minishell->env_list || !(t_list *)arg->node->arg || !((t_list *)arg->node->arg)->data)
	{
		code = export_no_arg(arg->minishell);
		free(p);
		return (code);
	}
	t_list *list_arg;
	list_arg = (t_list *)arg->node->arg;
	while (list_arg)
	{
		tmp = (char *)list_arg->data;
		if (tmp[0] == 0 || (!ft_isalpha(tmp[0]) && tmp[0] != '_'))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(tmp, 2);
			ft_putendl_fd("': not a valid identifier", 2);
			code = 2;
		}
		else
		{
			sep = ft_strchr(tmp, '=');
			int	i = 0;
			if (!ft_isalpha(tmp[i]) && tmp[i] != '_' && code != 2)
				code = 1;
			if (!sep && !code && tmp[i])
			{
				i++;
				while (tmp[i])
				{
					if (!ft_isalnum(tmp[i]) && tmp[i] != '_' && code != 2)
						code = 1;
					i++;
				}
				if (((t_list *)arg->node->arg)->next && code != 2)
				{
					ft_putstr_fd("minishell: export: `", 2);
					ft_putstr_fd(((t_list *)arg->node->arg)->next->data, 2);
					ft_putendl_fd("': not a valid identifier", 2);
					code = 2;
				}
			}
			else
			{
				i++;
				while (&tmp[i] != sep)
				{
					if (!ft_isalnum(tmp[i]) && tmp[i] != '_' && code != 2)
						code = 1;
					i++;
				}
			}
		}
		if (code == 1)
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(tmp, 2);
			ft_putendl_fd("': not a valid identifier", 2);
			code = 0;
		}
		else
		{
			t_var	*var;
			var = create_var(tmp, sep);
			if (!var)
			{
				free(p);
				return (-1);
			}
			safe_add_env_list(arg->minishell->env_list, var);
		}
		code = 0;
		list_arg = list_arg->next;
	}
	(void)local_var_to_env_var;
	free(p);
	return (code);
}
//Version non decoupée
/*static t_var	*create_var(char *str, char *sep)
{
	t_var	*var;
	int		len;

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
	len = ft_strlen(sep) - 1;
	if (len <= 0)
		var->val = NULL;
	else
	{
		var->val = malloc(sizeof(char) * len + 1);
		if (!var->val)
			return (NULL);
		ft_memcpy(var->val, sep + 1, len * sizeof(char));
		var->val[len] = 0;
	}
	return (var);
}*/
/*int	export_no_arg(t_env *minishell)
{
	t_list	*lst;
	t_var	*var;

	if (!minishell)
		return (-1);
	if (!minishell->env_list)
	{
		print_env_var_part2(minishell);
		return (0);
	}
	lst = minishell->env_list;
	while (lst)
	{
		var = (t_var *)lst->data;
		if (need_print(var->key))
		{
			ft_putstr_fd("export ", 1);
			if (var->key)
				ft_putstr_fd(var->key, 1);
			ft_putstr_fd("=\"", 1);
			if (var->val)
				ft_putstr_fd(var->val, 1);
			ft_putstr_fd("\"\n", 1);
		}
		lst = lst->next;
	}
	print_env_var_part2(minishell);
	return (0);
}*/

/*//AJOUT CELINE pour mettre var env dans ordre alpha
static t_list *sort_env_list_alpha(t_list *lst)
{
	t_list *sorted = NULL;
	t_var *var;
	t_list *new_node;
	t_list	*prev;
	t_list	*tmp;

	while (lst)
	{
		var = (t_var *)lst->data;
		new_node = ft_create_node(var);
		if (!sorted || ft_strncmp(((t_var *)sorted->data)->key, var->key, ft_strlen(var->key) + 1) > 0)
		{
			new_node->next = sorted;
			sorted = new_node;
		}
		else
		{
			prev = sorted;
			tmp = sorted->next;
			while (tmp && ft_strncmp(((t_var *)tmp->data)->key, var->key, ft_strlen(var->key) + 1) < 0)
			{
				prev = tmp;
				tmp = tmp->next;
			}
			new_node->next = tmp;
			prev->next = new_node;
		}
		lst = lst->next;
	}
	return sorted;
}
*/