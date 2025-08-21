/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 14:29:45 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/19 13:52:21 by cle-rouz         ###   ########.fr       */
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
			break;
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

void	exec_tree(t_env *minishell, t_tree *tree, t_tree *parent, int i, int *j)
{
	if (!tree)
		return ;
	if (tree->left)
		exec_tree(minishell, tree->left, tree, i + 1, j);
	if (i == 0 && !tree->type)
	{
		if (minishell->last_cmd)
			free(minishell->last_cmd);
		if (tree->built_in)
			update_last_cmd(minishell, ft_strndup(tree->content, ft_strlen(tree->content)));
		else
			update_last_cmd(minishell, ft_strndup(tree->content, ft_strlen(tree->content)));
		return ((void)(exec_one_cmd(minishell, tree)));
	}
	if (!tree->type || tree->status || (!tree->built_in && !tree->content))
		return ;
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
	{
		exec_pipeline(minishell, tree->right, 0, *j);
	}
	(*j)++;
}


//ESAI DE REDUCTION EXEC_TREE

/*typedef struct s_exec_params
{
	t_tree	*parent;
	int		i;
	int		*j;
}	t_exec_params;

static int	handle_root_node(t_env *minishell, t_tree *tree)
{
    if (minishell->last_cmd)
        free(minishell->last_cmd);
    minishell->last_cmd = ft_strndup(tree->content, ft_strlen(tree->content));
    exec_one_cmd(minishell, tree);
}

static int	should_skip_execution(t_tree *tree)
{
    if (!tree->type || tree->status || (!tree->built_in && !tree->content))
        return (1); // Doit skipper
    return (0);     // Ne doit pas skipper
}

static void	handle_left_pipeline(t_env *minishell, t_tree *tree, int *j)
{
	if (tree->left && !tree->left->type)
	{
		exec_pipeline(minishell, tree->left, 0, *j);
		(*j)++;
	}
}

static void	handle_right_pipeline(t_env *minishell, t_tree *tree, t_exec_params *params)
{
	if (!params->parent || !params->parent->type)
	{
		if (minishell->last_cmd)
			free(minishell->last_cmd);
		minishell->last_cmd = ft_strndup(tree->path, ft_strlen(tree->path));
		exec_pipeline(minishell, tree->right, 1, *(params->j));
	}
	else
		exec_pipeline(minishell, tree->right, 0, *(params->j));
	(*(params->j))++;
}

void	exec_tree(t_env *minishell, t_tree *tree, t_exec_params *params)
{
    if (!tree)
        return ;
    if (tree->left)
        exec_tree(minishell, tree->left, params);
    if (params->i == 0 && !tree->type)
    {
        handle_root_node(minishell, tree);
        return ; // Return après l'appel
    }
    if (should_skip_execution(tree))
        return ; // RETURN CONSERVÉ
    handle_left_pipeline(minishell, tree, params->j);
    handle_right_pipeline(minishell, tree, params);
}*/
