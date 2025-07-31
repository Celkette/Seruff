/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_one_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaporte <nlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 01:40:33 by nlaporte          #+#    #+#             */
/*   Updated: 2025/07/29 13:18:01 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

// creer un tableau de str depuis une liste
static char	**get_split_from_list(t_list *lst)
{
	char	**split;
	int		i;
	
	if (!lst)
		return (NULL);
	split = malloc(sizeof(char *) * (ft_lstsize(lst) + 1));
	if (!split)
		return (NULL);
	i = 0;
	while (lst)
	{
		split[i] = ft_strndup((char *)lst->data, ft_strlen((char *)lst->data));
		if (!split[i])
		{
			split[i] = 0;
			free_split(split);
			return (NULL);
		}
		lst = lst->next;
		i++;
	}
	split[i] = 0;
	return (split);
}

void	exec_cmd(t_env *minishell, t_tree *node)
{
	char	**tmp_arg;
	void	*arg;
	int		code;

	if (!minishell || !node || (!node->path && !node->built_in))
		return ;
	if (!node->built_in)
		ft_lstadd_front(&node->arg, ft_create_node(node->content));
	tmp_arg = get_split_from_list(node->arg);
	if (node->redir_in)
		use_redir_in(node);
	if (node->redir_out)
		use_redir_out(node);
	if (node->built_in)
	{
		code = 0;
		arg = get_arg_built_in(minishell, node);
		if (arg)
			code = (*node->built_in)(arg);
		exit(code);
	}
	else
		execve(node->path, tmp_arg, minishell->env);
	exit(0);
}

// Execute une commande simple
int	exec_one_cmd(t_env *minishell, t_tree *node)
{
	pid_t	pid;
	t_arg	*arg;

	if (!node || !node->content)
		return (-1);
	if (!node->built_in)
	{
		pid = fork();
		if (pid < 0)
			return (-2);
		if (pid == 0)
		{
			exec_cmd(minishell, node);
			exit(0);
		}
	}
	else
	{
		if (node->built_in)
		{
			arg = get_arg_built_in(minishell, node);
			if (arg)
				(*node->built_in)(arg);
			else
				(*node->built_in)(NULL);
			return (0);
		}
	}
	return (0);
}
