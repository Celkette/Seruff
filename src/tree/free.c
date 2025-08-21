/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 17:50:38 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/21 11:57:18 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static void	free_tree2(t_tree *tree)
{
	t_list	*to_free;

	while (tree->arg)
	{
		to_free = tree->arg;
		tree->arg = tree->arg->next;
		free(to_free->data);
		free(to_free);
	}
	if (tree->redir_in)
	{
		free(tree->redir_in->path);
		free(tree->redir_in);
	}
	if (tree->cmd_list)
		free_split(tree->cmd_list);
	if (tree->content)
		free(tree->content);
	if (tree->path)
		free(tree->path);
}

void	free_tree(t_tree *tree)
{
	t_list	*to_free;

	if (!tree)
		return ;
	if (tree->left)
		free_tree(tree->left);
	if (tree->right)
		free_tree(tree->right);
	while (tree->redir_out)
	{
		to_free = tree->redir_out;
		tree->redir_out = tree->redir_out->next;
		free(((t_r_out *)to_free->data)->path);
		free(to_free->data);
		free(to_free);
	}
	free_tree2(tree);
	free(tree);
}
