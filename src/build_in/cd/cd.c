/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 17:06:24 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/21 12:52:52 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

char	*manage_tilee(t_env *minishell, const char *path)
{
	char	*str;
	int		total_len;

	if (!minishell->user.home)
	{
		if (ft_strlen((char *)path) == 1)
			return (ft_putstr_fd("\n", 2), (NULL));
		*(char *)path = ' ';
		return (NULL);
	}
	total_len = ft_strlen(minishell->user.home) + ft_strlen((char *)path);
	str = malloc(sizeof(char) * (total_len) + 1);
	if (!str)
		return (NULL);
	ft_memcpy(str, minishell->user.home, \
		sizeof(char) * (ft_strlen(minishell->user.home)));
	ft_memcpy(&str[ft_strlen(minishell->user.home)], &path[1], \
		sizeof(char) * (ft_strlen((char *)path)));
	*(str + total_len) = 0;
	if (!str)
		return (0);
	return (str);
}

char	*get_old_path(t_arg *arg)
{
	char	*new_path;

	new_path = NULL;
	if (!arg)
		return (NULL);
	if (!arg->minishell->last_path)
	{
		new_path = ft_strndup(var_env_value(arg->minishell, "HOME"), \
			ft_strlen(var_env_value(arg->minishell, "HOME")));
	}
	else
		new_path = ft_strndup(arg->minishell->last_path, \
			ft_strlen(arg->minishell->last_path));
	if (!new_path)
		return (NULL);
	ft_putendl_fd(new_path, 1);
	return (new_path);
}

char	*get_path(t_arg *arg)
{
	char	*new_path;

	new_path = NULL;
	if (!arg)
		return (NULL);
	if (!arg->node->arg)
	{
		if (!var_env_value(arg->minishell, "HOME"))
		{
			ft_putendl_fd("bash: cd: HOME not set", 2);
			return (NULL);
		}
		else
			new_path = ft_strndup(var_env_value(arg->minishell, "HOME"), \
			ft_strlen(var_env_value(arg->minishell, "HOME")));
	}
	else if (((char *)arg->node->arg->data)[0] == '~')
		new_path = manage_tilee(arg->minishell, arg->node->arg->data);
	else if ((((char *)arg->node->arg->data)[0] == '-' && \
			((char *)arg->node->arg->data)[1] == 0))
		new_path = get_old_path(arg);
	else
		new_path = ft_strndup(arg->node->arg->data, \
			ft_strlen(arg->node->arg->data));
	return (new_path);
}

static void	handle_get_new_path(t_env *minishell)
{
	t_var	*var;

	if (minishell->act_path)
	{
		var = create_classic_var("PWD", minishell->act_path);
		if (var)
			safe_add_env_list(minishell->env_list, var);
		var = create_classic_var("OLDPWD", minishell->last_path);
		if (var)
			safe_add_env_list(minishell->env_list, var);
	}
}

int	get_new_path(t_env *minishell)
{
	int		size;

	if (!minishell)
		return (1);
	size = 0;
	while (1)
	{
		size += 100;
		if (size >= INT_MAX - 101)
			return (1);
		minishell->act_path = malloc(sizeof(char) * size);
		if (!minishell->act_path)
			return (1);
		if (!getcwd(minishell->act_path, size - 1))
		{
			if (errno != ERANGE)
				return (1);
		}
		else
			break ;
	}
	handle_get_new_path(minishell);
	return (100);
}