/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaporte <nlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:27:16 by nlaporte          #+#    #+#             */
/*   Updated: 2025/07/29 14:10:29 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

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

static t_var	*create_var(char *str, char *sep)
{
	t_var	*var;
	int		len;

	if (!str || !sep)
		return (NULL);
	var = malloc(sizeof(t_var));
	if (!var)
		return (NULL);
	var->key = malloc(sizeof(char) * (sep - str + 1));
	if (!var->key)
		return (NULL);
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
}

static int	local_var_to_env_var(t_env *minishell, t_tree *node)
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

int	export_no_arg(t_env *minishell)
{
	t_list	*lst;
	t_var	*var;

	if (!minishell || !minishell->env_list)
		return (-1);
	lst = minishell->env_list;
	while (lst)
	{
		var = (t_var *)lst->data;
		ft_putstr_fd("export ", 1);
		if (var->key)
			ft_putstr_fd(var->key, 1);
		ft_putstr_fd("=", 1);
		if (var->val)
			ft_putstr_fd(var->val, 1);
		ft_putstr_fd("\n", 1);
		lst = lst->next;
	}
	return (0);
}

static int	as_equal(t_env *minishell, t_tree *node)
{
	t_var	*new;
	t_var	*old;

	if (!minishell || !node)
		return (-1);
	new = create_var(node->arg->data, ft_strchr(node->arg->data, '='));
	if (!new)
		return (-1);
	old = get_var(minishell->env_list, new->key);
	if (!old)
	{
		ft_lstadd_back(&minishell->env_list, ft_create_node(new));
		return (0);
	}
	free(old->val);
	old->val = new->val;
	free(new->key);
	free(new);
	return (0);
}

int	export_built_in(void *p)
{
	t_arg	*arg;
	void	 *tmp;
	int		code;

	if (!p)
		return (-1);
	arg = (t_arg *)p;
	if (!arg->node || !arg->minishell || !arg->minishell->env_list)
	{
		free(p);
		return (-1);
	}
	if (!(t_list *)arg->node->arg)
	{
		code = export_no_arg(arg->minishell);
		free(p);
		return (code);
	}
	tmp = ((t_list *)arg->node->arg)->data;
	if (!tmp)
		code = export_no_arg(arg->minishell);
	else if (ft_strchr(tmp, '='))
		code = as_equal(arg->minishell, arg->node);
	else
		code = local_var_to_env_var(arg->minishell, arg->node);
	free(p);
	return (code);
}
