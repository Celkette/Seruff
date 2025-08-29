/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 14:29:45 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/21 13:57:34 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

// Permet de rediriger vers le node de larbre vers la bonne fn dexec
int	exec_one_tree(t_env *minishell, t_tree *node)
{
	if (!node || !minishell)
		return (-1);
	return (0);
}

void	update_last_cmd(t_env *minishell, char *s)
{
	t_list	*lst;
	t_var	*tmp;
	t_var	*var;

	if (!minishell)
		return ;
	lst = minishell->env_list;
	while (lst)
	{
		tmp = lst->data;
		if (tmp && *tmp->key == '_' && *(tmp->key + 1) == 0)
			break ;
		lst = lst->next;
	}
	if (lst)
	{
		free(tmp->val);
		tmp->val = s;
		return ;
	}
	var = create_classic_var("_", s);
	ft_lstadd_back(&minishell->env_list, ft_create_node(var));
	return ;
}

int	exec_tree(t_env *minishell, t_tree *tree, t_tree *parent, int *j)
{
	if (!tree)
		return (127);
	if (tree->left)
		exec_tree(minishell, tree->left, tree, j);
	if (!tree->type || tree->status || (!tree->built_in && !tree->content))
		return (0);
	if (tree->left && !tree->left->type)
	{
		exec_pipeline(minishell, tree->left, 0, *j);
		(*j)++;
	}
	if (!parent || !parent->type)
	{
		if (minishell->last_cmd)
			free(minishell->last_cmd);
		minishell->last_cmd = ft_strndup(tree->path, ft_strlen(tree->path));
		exec_pipeline(minishell, tree->right, 1, *j);
	}
	else
		exec_pipeline(minishell, tree->right, 0, *j);
	(*j)++;
	return (0);
}
