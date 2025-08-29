/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 13:10:23 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/29 10:47:03 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

t_var	*create_var_no_value(char *str)
{
	t_var	*var;

	if (!str || (*str == '_' && *(str + 1) == 0))
		return (NULL);
	var = malloc(sizeof(t_var));
	if (!var)
		return (NULL);
	var->key = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!var->key)
	{
		free(var);
		return (NULL);
	}
	ft_memcpy(var->key, str, ft_strlen(str) * sizeof(char));
	var->key[ft_strlen(str)] = 0;
	var->val = NULL;  // Pas de valeur pour export VAR
	return (var);
}

static void	print_export_no_arg(void *p, t_list *lst)
{
	t_var	*var;

	if (((t_arg *)p)->node && ((t_arg *)p)->node->redir_out)
		if (use_redir_out(((t_arg *)p)->node) != 0)
			return ;
	while (lst)
	{
		var = (t_var *)lst->data;
		if (var)
		{
			ft_putstr_fd("export ", 1);
			if (var->key)
				ft_putstr_fd(var->key, 1);
			if (var->val != NULL)
			{
				ft_putstr_fd("=\"", 1);
				ft_putstr_fd(var->val, 1);
				ft_putstr_fd("\"", 1);
			}
			ft_putstr_fd("\n", 1);
		}
		lst = lst->next;
	}
	if (!((t_arg *)p)->node->top && ((t_arg *)p)->node->stdout_cpy != 0)
		dup_close(((t_arg *)p)->node->status, STDOUT_FILENO);
}

int	export_no_arg(void *p, t_env *minishell)
{
	t_list	*lst;

	if (!minishell)
		return (-1);
	if (!minishell->env_list)
		return (0);
	lst = minishell->env_list;
	print_export_no_arg(p, lst);
	return (0);
}
