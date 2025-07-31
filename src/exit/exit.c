/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaporte <nlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:37:57 by nlaporte          #+#    #+#             */
/*   Updated: 2025/07/30 10:02:02 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	exit_minishell2(t_env *minishell)
{
	free(minishell->act_path);
	if (minishell->old_path)
		free(minishell->old_path);
	if (minishell->pid)
		free(minishell->pid);
	if (minishell->path_fun_split)
		free_split(minishell->path_fun_split);
	rl_clear_history();
	clear_env_var(minishell->env_list);
	return ;
}

void	exit_minishell(t_env *minishell)
{
	free(minishell->act_path);
	if (minishell->old_path)
		free(minishell->old_path);
	if (minishell->pid)
		free(minishell->pid);
	if (minishell->path_fun_split)
		free_split(minishell->path_fun_split);
	rl_clear_history();
	if (minishell->tree)
		free_tree(minishell->tree);
	minishell->tree = NULL;
	clear_env_var(minishell->env_list);
	return ;
}

int	exit_minishell_built_in(void *p)
{
	if (!p)
		exit(0);
	exit_minishell(((t_arg *)p)->minishell);
	free(p);
	exit(0);
}
