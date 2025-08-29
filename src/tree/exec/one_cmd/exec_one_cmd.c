/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_one_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 01:40:33 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/28 21:59:34 by Seth         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"
#include <stdio.h>

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

void	set_use_redir(t_env *minishell, char **tmp_arg, t_tree *node)
{
	if (node->redir_out)
		if (use_redir_out(node) != 0)
			(exit_minishell(minishell), free_split(tmp_arg), exit(1));
	if (node->redir_in)
		use_redir_in(node);
	if (node->redir_in && node->redir_in->fd)
		close(node->redir_in->fd);
}

void	exec_cmd(t_env *minishell, t_tree *node, int code)
{
	char	**tmp_arg;
	void	*arg;

	check_exec_cmd(minishell, node);
	(signal(SIGQUIT, SIG_DFL), signal(SIGINT, sig_ctrl_c_catcher2));
	if (!node->built_in)
	{
		(ft_lstadd_front(&node->arg, \
		ft_create_node(ft_strndup(node->content, ft_strlen(node->content)))));
		tmp_arg = get_split_from_list(node->arg);
		set_use_redir(minishell, tmp_arg, node);
	}
	if (node->built_in)
	{
		arg = get_arg_built_in(minishell, node);
		code = (*node->built_in)(arg);
		if (arg && (code != 0 || errno == 2))
			free(arg);
		(exit_minishell_fork(minishell), exit(code));
	}
	if (minishell->env)
		execve(node->path, tmp_arg, minishell->env);
	else
		execve(node->path, tmp_arg, NULL);
	(free_split(tmp_arg), exit_minishell_fork(minishell));
	exit(0);
}

static int	manage_fork(t_env *minishell, t_tree *node)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (-2);
	if (pid == 0)
		exec_cmd(minishell, node, 0);
	if (node->redir_in && node->redir_in->fd > 0)
		close(node->redir_in->fd);
	node->pid = pid;
	return (0);
}

// Execute une commande simple
int	exec_one_cmd(t_env *minishell, t_tree *node)
{
	t_arg	*arg;
	int		code;

	if (!node || !node->content)
	{
		if (node->redir_in && node->redir_in->fd > 0)
			close(node->redir_in->fd);
		return (127);
	}
	if (node->need_parent)
	{
		arg = get_arg_built_in(minishell, node);
		if (arg)
		{
			code = (*node->built_in)(arg);
			if (arg && (code != 0 || errno == 2))
				free(arg);
			return (code);
		}
		else
			return ((*node->built_in)(NULL));
	}
	return (manage_fork(minishell, node));
}
