/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:05:29 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/04/01 13:07:49 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_env_path(char **env)
{
	size_t	i;
	char	*env_path;

	i = -1;
	env_path = NULL;
	if (!env || !*env)
	{
		pipex_error_i("\033[0;31m Environnement inexistant\e[0m", -1);
	}
	while (env[++i] != NULL)
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			env_path = env[i] + 5;
			break ;
		}
	}
	if (env_path == NULL)
		pipex_error_i("\033[31m PATH non trouvé\n\e[0m", -1);
	return (env_path);
}

char	**get_env_tab(char **env)
{
	char	*env_path;
	char	**env_tab_path;

	env_path = get_env_path(env);
	env_tab_path = ft_split(env_path, ':');
	if (env_tab_path == NULL)
	{
		ft_free_tab(env_tab_path);
		pipex_error_i("ft_split(in get_env_tab) failed", -1);
	}
	return (env_tab_path);
}

char	*create_full_path(char *path, char *cmd)
{
	char	*temp_path;
	char	*path_cmd;

	temp_path = NULL;
	path_cmd = NULL;
	if (!path || !cmd || *cmd == '\0')
		return (NULL);
	temp_path = ft_strjoin(path, "/");
	if (temp_path == NULL)
		pipex_error_i("temp_path failed (in create_full_path)", -1);
	path_cmd = ft_strjoin(temp_path, cmd);
	free(temp_path);
	if (path_cmd == NULL)
		pipex_error_i("path_cmd failed (in create_full_path)", -1);
	return (path_cmd);
}

static void	ft_annexe(char **cmd, char **tab)
{
	ft_free_tab(cmd);
	ft_free_tab(tab);
}

char	*get_cmd_path(char *cmd, char **env)
{
	int		i;
	char	*path_cmd;
	char	**cmd_args;
	char	**env_tab;
	char	*str_cmd_args;

	i = 0;
	path_cmd = NULL;
	cmd_args = get_cmd_arg(cmd);
	env_tab = get_env_tab(env);
	if (!env_tab)
		return (ft_free_tab(cmd_args), NULL);
	while (env_tab[i++] != NULL)
	{
		path_cmd = create_full_path(env_tab[i], cmd_args[0]);
		if (check_cmd(path_cmd) == 1)
			return (ft_annexe(cmd_args, env_tab), path_cmd);
		free(path_cmd);
	}
	if (check_cmd(cmd_args[0]) == 1)
	{
		str_cmd_args = ft_strdup(cmd_args[0]);
		return (ft_annexe(cmd_args, env_tab), str_cmd_args);
	}
	return (ft_annexe(cmd_args, env_tab), NULL);
}
