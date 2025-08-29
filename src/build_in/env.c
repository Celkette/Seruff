/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 20:43:35 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/28 20:01:16 by Seth         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static void	print_env_list(t_list *lst)//modifié 21-26
{
	t_var	*var;

	while (lst)
	{
		var = (t_var *)lst->data;
		if (var && var->val != NULL)  // Ne pas afficher les variables sans valeur
		{
			ft_putstr_fd(var->key, 1);
			ft_putstr_fd("=", 1);
			ft_putstr_fd(var->val, 1);
			ft_putstr_fd("\n", 1);
		}
		lst = lst->next;
	}
}

void	dup_close(int fd, int fd2)
{
	dup2(fd, fd2);
	close(fd);
}

int	env(void *p)
{
	t_env	*minishell;
	t_list	*lst;

	if (!p)
		return (1);
	minishell = ((t_arg *)p)->minishell;
	if (!minishell->env_list)
	{
		if (((t_arg *)p)->node->top)
			exit_minishell(((t_arg *)p)->minishell);
		free(p);
		return (1);
	}
	if (((t_arg *)p)->node && ((t_arg *)p)->node->redir_out)
		if (use_redir_out(((t_arg *)p)->node) != 0)
			return (1);
	lst = minishell->env_list;
	print_env_list(lst);
	if (!((t_arg *)p)->node->top && ((t_arg *)p)->node->stdout_cpy != 0)
		dup_close(((t_arg *)p)->node->status, STDOUT_FILENO);
	if (!((t_arg *)p)->node->top && ((t_arg *)p)->node->stdin_cpy != 0)
		dup_close(((t_arg *)p)->node->status, STDIN_FILENO);
	if (((t_arg *)p)->node->top)
		(exit_minishell(((t_arg *)p)->minishell));
	return ((free(p), 0));
}
