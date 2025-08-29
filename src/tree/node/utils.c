/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:02:12 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/28 23:14:01 by Seth         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static int	get_cmd_abs_path(t_env *minishell, char *cmd)
{
	if (!minishell || !cmd)
		return (-1);
	if (access(cmd, X_OK) == 0)
		return (1);
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(" ", 2);
	ft_putendl_fd(strerror(errno), 2);
	return (-1);
}

static char	*get_cmd_path(t_env *minishell, char *cmd, int i)
{
	char	*tmp_path;

	if (!minishell || !cmd || !minishell->path_fun_split || \
		!minishell->path_fun_split[i])
		return (NULL);
	tmp_path = malloc(sizeof(char) * \
	(ft_strlen(minishell->path_fun_split[i]) + ft_strlen(cmd) + 2));
	if (!tmp_path)
		return (NULL);
	ft_memcpy(tmp_path, minishell->path_fun_split[i], \
	ft_strlen(minishell->path_fun_split[i]));
	*(tmp_path + ft_strlen(minishell->path_fun_split[i])) = '/';
	ft_memcpy(&tmp_path[ft_strlen(minishell->path_fun_split[i]) + \
	1], cmd, ft_strlen(cmd));
	*(tmp_path + ft_strlen(minishell->path_fun_split[i]) + \
	ft_strlen(cmd) + 1) = 0;
	return (tmp_path);
}

void	update_path(t_env *minishell)
{
	t_var	*path_env_var;

	if (!minishell)
		return ;
	path_env_var = get_env_var(minishell, "PATH");
	if (minishell->path_fun_split)
		free_split(minishell->path_fun_split);
	if (!path_env_var)
	{
		minishell->path_fun_split = NULL;
		return ;
	}
	if (!path_env_var->val || *path_env_var->val == '\0')
	{
		minishell->path_fun_split = NULL;
		return ;
	}
	minishell->path_fun_split = ft_split(path_env_var->val, ':');
}

char	*cmd_is_in_path(t_env *minishell, char *cmd)
{
	char	*tmp_path;
	int		i;

	if (!minishell || !cmd || !*cmd)
		return (0);
	update_path(minishell);
	if (ft_strchr(cmd, '/') && get_cmd_abs_path(minishell, cmd))
		return (ft_strndup(cmd, ft_strlen(cmd)));
	if (!minishell->path_fun_split)
		return (NULL);
	i = 0;
	while (minishell->path_fun_split[i])
	{
		tmp_path = get_cmd_path(minishell, cmd, i);
		if (tmp_path && access(tmp_path, X_OK) == 0)
			break ;
		free(tmp_path);
		tmp_path = NULL;
		i++;
	}
	if (tmp_path == NULL)
		return (NULL);
	return (tmp_path);
}

// fonction qui retourne un pointeur de fonction dune commande built_in
int	(*is_built_in(t_env *minishell, t_tree *node, char *str))(void *p)
{
	if (!minishell || !node || !str)
		return (NULL);
	if (ft_strncmp(str, "echo", ft_strlen("echo") + 1) == 0)
		return ((node->need_parent = 1, &echoo));
	else if (ft_strncmp(str, "cd", ft_strlen("cd") + 1) == 0)
		return ((node->need_parent = 1, &cd));
	else if (ft_strncmp(str, "pwd", ft_strlen("pwd") + 1) == 0)
		return ((node->need_parent = 1, &pwd));
	else if (ft_strncmp(str, "export", ft_strlen("export") + 1) == 0)
		return ((node->need_parent = 1, &export_built_in));
	else if (ft_strncmp(str, "unset", ft_strlen("unset") + 1) == 0)
		return ((node->need_parent = 1, &unset_built_in));
	else if (ft_strncmp(str, "env", ft_strlen("env") + 1) == 0)
		return ((node->need_parent = 1, &env));
	else if (ft_strncmp(str, "exit", ft_strlen("exit") + 1) == 0)
		return ((node->need_parent = 1, &exit_minishell_built_in));
	return (NULL);
}
