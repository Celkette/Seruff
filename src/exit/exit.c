/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaporte <nlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:37:57 by nlaporte          #+#    #+#             */
/*   Updated: 2025/07/28 13:00:35 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

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
	return ;
}
