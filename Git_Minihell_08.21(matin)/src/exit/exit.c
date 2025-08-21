/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:37:57 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/19 13:23:18 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static void	close_fd_minishell(t_env *minishell)
{
	if (minishell->exec.fd[0] > 0)
		close(minishell->exec.fd[0]);
	if (minishell->exec.fd[1] > 0)
		close(minishell->exec.fd[1]);
	if (minishell->exec.fd2[0] > 0)
		close(minishell->exec.fd2[0]);
	if (minishell->exec.fd2[1] > 0)
		close(minishell->exec.fd2[1]);
}

static void	exit_minishell2(t_env *minishell)
{
	if (!minishell)
		return ;
	if (minishell->token_list)
	{
		free_token_list(minishell->token_list);
		minishell->token_list = NULL;
	}
	if (minishell->var)
	{
		free_var_list(minishell->var);
		minishell->var = NULL;
	}
	minishell->token_list = NULL;
	if (minishell->env_list)
	{
		clear_env_var(minishell->env_list);
		minishell->env_list = NULL;
	}
}

void	exit_minishell(t_env *minishell)
{
	free(minishell->act_path);
	if (minishell->last_path)
		free(minishell->last_path);
	if (minishell->pid)
		free(minishell->pid);
	if (minishell->path_fun_split)
		free_split(minishell->path_fun_split);
	rl_clear_history();
	if (minishell->last_cmd)
		free(minishell->last_cmd);
	if (minishell->tree)
	{
		free_tree(minishell->tree);
		minishell->tree = NULL;
	}
	exit_minishell2(minishell);
	close_fd_minishell(minishell);
	return ;
}

int	exit_minishell_built_in(void *p)
{
	t_arg	*arg;
	int		code;

	if (!p)
	{
		exit_minishell(((t_arg *)p)->minishell);
		exit(0);
	}
	arg = (t_arg *)p;
	code = 0;
	if (arg->node->arg && (char *)arg->node->arg->data)
		code = ft_atoi(arg->node->arg->data);
	exit_minishell(((t_arg *)p)->minishell);
	free(p);
	exit(code);
}
