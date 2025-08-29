/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celine <celine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:12:33 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/28 20:12:40 by Seth         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

int	del_node(t_list **head, t_list *lst)
{
	if (!lst || !head || !*head)
		return (-1);
	if (lst == *head)
	{
		*head = lst->next;
		if ((*head)->prev)
			(*head)->prev = NULL;
	}
	else
	{
		if (lst->next)
			lst->next->prev = lst->prev;
		if (lst->prev)
			lst->prev->next = lst->next;
	}
	free(((t_var *)lst->data)->key);
	free(((t_var *)lst->data)->val);
	free(lst->data);
	free(lst);
	return (0);
}

static t_list	*get_env_var_lst(t_env *minishell, char *var_name)
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
			return (lst);
		lst = lst->next;
	}
	return (NULL);
}

int	check_unset(void *p, t_arg *arg)
{
	if (!arg->minishell || !arg->node || !arg->node->arg)
	{
		if (((t_arg *)p)->node->top)
			exit_minishell(((t_arg *)p)->minishell);
		free(p);
		return (1);
	}
	return (0);
}

int	unset_built_in(void *p)
{
	int		code;
	t_arg	*arg;
	t_list	*arg_node;
	t_list	*to_del;

	if (!p)
		return (-1);
	code = 0;
	arg = (t_arg *)p;
	if (check_unset(p, arg))
		return (1);
	arg_node = arg->node->arg;
	while (arg_node)
	{
		to_del = get_env_var_lst(arg->minishell, arg_node->data);
		if (to_del && ft_strncmp(((t_var *)to_del->data)->key, "_", 2) != 0)
			del_node(&arg->minishell->env_list, to_del);
		arg_node = arg_node->next;
	}
	if (((t_arg *)p)->node->top)
		exit_minishell(((t_arg *)p)->minishell);
	free(p);
	return (code);
}
