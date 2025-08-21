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
		return (-1);
	*arg = (t_arg *)p;
	if (!(*arg)->minishell || !(*arg)->node)
	{
		free(p);
		return (-1);
	}
	return (0);
}

static int	handle_cd_path(t_arg *arg, char **new_path)
{
	*new_path = get_path(arg);
	if (!(*new_path))
		return (-1);
	if (make_chdir(arg, *new_path) != 0)
		return (-1);
	return (0);
}

static int	update_last_path(t_arg *arg)
{
	if (arg->minishell->last_path)
		free(arg->minishell->last_path);
	arg->minishell->last_path = ft_strndup(arg->minishell->act_path,
			ft_strlen(arg->minishell->act_path));
	if (!arg->minishell->last_path)
		return (-1);
	return (0);
}

static void	finalize_cd(t_arg *arg)
{
	if (arg->minishell->act_path)
		free(arg->minishell->act_path);
	get_new_path(arg->minishell);
	update_env_oldpath(arg->minishell);
}

int	cd(void *p)
{
	t_arg	*arg;
	char	*new_path;

	arg = NULL;
	new_path = NULL;
	if (validate_cd_args(p, &arg) != 0)
		return (-1);
	if (handle_cd_path(arg, &new_path) != 0)
	{
		free(p);
		return (-1);
	}
	if (update_last_path(arg) != 0)
	{
		free(p);
		return (-1);
	}
	finalize_cd(arg);
	free(p);
	free(new_path);
	return (0);
}

//Version sans decoupage
/*int	cd(void *p)
{
	t_arg	*arg;
	char	*new_path;

	if (!p)
		return (-1);
	arg = (t_arg *)p;
	new_path = NULL;
	if (!arg->minishell || !arg->node)
	{
		free(p);
		return (-1);
	}
	new_path = get_path(arg);
	if (!new_path)
	{
		free(p);
		return (-1);
	}
	if (make_chdir(arg, new_path) != 0)
	{
		free(p);
		return (-1);
	}
	if (arg->minishell->last_path)
		free(arg->minishell->last_path);
	arg->minishell->last_path = ft_strndup(arg->minishell->act_path,
		ft_strlen(arg->minishell->act_path));
	if (!arg->minishell->last_path)
	{
		free(p);
		return (-1);
	}
	if (arg->minishell->act_path)
		free(arg->minishell->act_path);
	get_new_path(arg->minishell);
	update_env_oldpath(arg->minishell);
	free(p);
	free(new_path);
	return (0);
}*/
