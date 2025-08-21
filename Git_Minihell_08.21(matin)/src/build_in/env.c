/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 20:43:35 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/19 15:24:54 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static void	print_env_list(t_list *lst)
{
	t_var	*var;

	while (lst)
	{
		var = (t_var *)lst->data;
		if (var)
		{
			ft_putstr_fd(var->key, 1);
			ft_putstr_fd("=", 1);
			if (var->val)
				ft_putstr_fd(var->val, 1);
			ft_putstr_fd("\n", 1);
		}
		lst = lst->next;
	}
}

int	env(void *p)
{
	t_env	*minishell;
	t_list	*lst;

	if (!p)
		return (-1);
	minishell = ((t_arg *)p)->minishell;
	if (!minishell->env_list)
	{
		free(p);
		return (-1);
	}
	if (((t_arg *)p)->node && ((t_arg *)p)->node->redir_out)
		use_redir_out(((t_arg *)p)->node);
	lst = minishell->env_list;
	print_env_list(lst);
	free(p);
	return (0);
}
/*int	env(void *p)
{
	t_env	*minishell;
	t_list	*lst;
	t_var	*var;

	if (!p)
		return (-1);
	minishell = ((t_arg *)p)->minishell;
	if (!minishell->env_list)
	{
		env_print2(minishell);
		free(p);
		return (-1);
	}
	if (((t_arg *)p)->node && ((t_arg *)p)->node->redir_out)
		use_redir_out(((t_arg *)p)->node);
	lst = minishell->env_list;
	while (lst)
	{
		var = (t_var *)lst->data;
		if (var && need_print(var->key))
		{
			ft_putstr_fd(var->key, 1);
			ft_putstr_fd("=", 1);
			if (var->val)
				ft_putstr_fd(var->val, 1);
			ft_putstr_fd("\n", 1);
		}
		lst = lst->next;
	}
	env_print2(minishell);
	free(p);
	return (0);
}
*/
