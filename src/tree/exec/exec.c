/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaporte <nlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 14:29:45 by nlaporte          #+#    #+#             */
/*   Updated: 2025/07/29 05:03:34 by nlaporte         ###   ########.fr       */
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

void	exec_tree(t_env *minishell, t_tree *tree, t_tree *parent, int i, int *j)
{
	if (!tree)
		return ;
	if (tree->left)
		exec_tree(minishell, tree->left, tree, i + 1, j);
	if (i == 0 && !tree->type)
		return (void)(exec_one_cmd(minishell, tree));
	if (!tree->type || tree->status)
		return ;
	if (tree->left && !tree->left->type)
	{
		exec_pipeline(minishell, tree->left, 0, *j);
		(*j)++;
	}
	if (!parent || !parent->type)
		exec_pipeline(minishell, tree->right, 1, *j);
	else
		exec_pipeline(minishell, tree->right, 0, *j);
	(*j)++;
	(void)parent;
}
