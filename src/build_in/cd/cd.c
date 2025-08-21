/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 17:06:24 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/21 11:19:59 by cle-rouz         ###   ########.fr       */
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
		{
			printf("\n");
			return (NULL);
		}
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

int	make_chdir(t_arg *arg, char *new_path)
{
	if (!new_path)
		return (-1);
	else if (chdir(new_path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(arg->node->arg->data, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
		free(new_path);
		return (-1);
	}
	return (0);
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
	else if ((((char *)arg->node->arg->data)[0] == '~'))
		new_path = manage_tilee(arg->minishell, arg->node->arg->data);
	else if ((((char *)arg->node->arg->data)[0] == '-' && \
			((char *)arg->node->arg->data)[1] == 0))
		new_path = get_old_path(arg);
	else
		new_path = ft_strndup(arg->node->arg->data, \
			ft_strlen(arg->node->arg->data));
	return (new_path);
}

void	get_new_path(t_env *minishell)
{
	int		size;
	t_var	*var;

	if (!minishell)
		return ;
	size = 0;
	while (1)
	{
		size += 100;
		if (size >= INT_MAX - 101)
			return ;
		minishell->act_path = malloc(sizeof(char) * size);
		if (!minishell->act_path)
			return ;
		if (!getcwd(minishell->act_path, size - 1))
		{
			if (errno != ERANGE)
				return ;
		}
		else
			break ;
	}
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

//Version avec commentaires
/*static int	make_chdir(t_arg *arg, char *new_path)
{
	if (!new_path)
		return (-1);
	else if (chdir(new_path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(arg->node->arg->data, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
		free(new_path);
		return (-1);
	}
	
	//if (arg->minishell->last_path)
	//	free(arg->minishell->last_path);
	//arg->minishell->last_path = ft_strndup(arg->minishell->act_path, \
	//	ft_strlen(arg->minishell->act_path));
	//if (arg->minishell->act_path)
	//	free(arg->minishell->act_path);
	//arg->minishell->act_path = ft_strndup(new_path, ft_strlen(new_path));
	//free(new_path);
	
	return (0);
}*/
