/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:37:57 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/29 01:19:33 by Seth         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

int	exit_minishell_built_in(void *p)
{
	t_arg	*arg;
	int		code;

	if (!p)
		exit(0);
	ft_putendl_fd("exit", 1);
	arg = (t_arg *)p;
	code = manage_error_exit(p, arg->node->arg);
	if (code != 0)
	{
		free(p);
		return (code);
	}
	if (arg->node->arg && (char *)arg->node->arg->data)
		code = ft_atoi(arg->node->arg->data);
	exit_minishell(((t_arg *)p)->minishell);
	free(p);
	exit(code);
}

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
	close(minishell->stdin_cpy);
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

void	exit_minishell_fork(t_env *minishell)
{
	char	*args[1];

	args[0] = NULL;
	free(minishell->act_path);
	close(minishell->stdin_cpy);
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
	execve("/", args, NULL);
}
