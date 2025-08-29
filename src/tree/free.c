/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 17:50:38 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/28 22:12:41 by Seth         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	free_redir_in(t_r_in *redir_in)
{
	free(redir_in->path);
	if (redir_in->file)
	{
		unlink(redir_in->file);
		free(redir_in->file);
	}
	if (redir_in->fd > 0)
		close(redir_in->fd);
	free(redir_in);
}

static void	free_tree2(t_tree *tree)
{
	t_list	*to_free;

	if (tree->content)
		free(tree->content);
	while (tree->arg)
	{
		to_free = tree->arg;
		tree->arg = tree->arg->next;
		if (to_free->data)
			free(to_free->data);
		free(to_free);
	}
	if (tree->redir_in)
		free_redir_in(tree->redir_in);
	if (tree->cmd_list)
		free_split(tree->cmd_list);
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
