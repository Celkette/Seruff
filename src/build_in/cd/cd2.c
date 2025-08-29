/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 09:10:11 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/08/19 12:18:31 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

/********************************
 * int	cd(void *p) decoupé     *
*********************************/
static int	validate_cd_args(void *p, t_arg **arg)
{
	*arg = NULL;
	if (!p)
		return (1);
	*arg = (t_arg *)p;
	if (!(*arg)->minishell || !(*arg)->node)
	{
		free(p);
		return (1);
	}
	return (0);
}

static int	handle_cd_path(t_arg *arg, char **new_path)
{
	*new_path = get_path(arg);
	if (!(*new_path))
		return (1);
	if (make_chdir(arg, *new_path) != 0)
		return (1);
	return (0);
}

static int	update_last_path(t_arg *arg)
{
	if (arg->minishell->last_path)
		free(arg->minishell->last_path);
	arg->minishell->last_path = ft_strndup(arg->minishell->act_path,
			ft_strlen(arg->minishell->act_path));
	if (!arg->minishell->last_path)
		return (1);
	return (0);
}

static int	finalize_cd(t_arg *arg)
{
	if (arg->minishell->act_path)
		free(arg->minishell->act_path);
	if (!get_new_path(arg->minishell))
		return (1);
	update_env_oldpath(arg->minishell);
	return (0);
}

int	first_part_cd(void *p, int *code, t_arg *arg, char *new_path)
{
	if (((t_arg *)p)->node && ((t_arg *)p)->node->redir_out)
		if (use_redir_out(((t_arg *)p)->node) != 0)
			return (1);
	if (((t_arg *)p)->node->arg && ((t_arg *)p)->node->arg->next)
	{
		ft_putendl_fd("minishell: cd: too many arguments", 2);
		if (((t_arg *)p)->node->top)
		{
			exit_minishell(((t_arg *)p)->minishell);
			exit(1);
		}
		return (1);
	}
	if (validate_cd_args(p, &arg) != 0)
		*code = 1;
	if (handle_cd_path(arg, &new_path) != 0)
		*code = 1;
	if (update_last_path(arg) != 0)
		*code = 1;
	if (finalize_cd(arg))
		*code = 1;
	return (*code);
}