/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celine <celine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 17:10:40 by nlaporte          #+#    #+#             */
/*   Updated: 2025/07/29 14:20:48 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

/*
 * Recupere le PATH et le stock dans la struct minishell
*/
static int	get_path(t_env *minishell)
{
	char	**split_path;
	char	**env;

	split_path = NULL;
	env = minishell->env;
	while (*minishell->env)
	{
		if (ft_strncmp(*minishell->env, "PATH=", 5) == 0)
		{
			// *minishell->env + 5 permet davancer de 5 caractere dans la chaine pour skip le debut (PATH=)
			split_path = ft_split(*minishell->env + 5, ':');
			if (!split_path)
			{
				minishell->env = env;
				return (-1);
			}
			break ;
		}
		minishell->env++;
	}
	if (split_path)
		minishell->path_fun_split = split_path;
	else
		minishell->path_fun_split = NULL;
	minishell->env = env;
	return (0);
}

/*
 * Import le pid de minishell
 * va dans /proc/self/status
 * et cherche la ligne qui commence
 * par "Pid:" avance jusquau premier
 * chiffre puis copie le reste de la
 * chaine dans minishell->pid
*/

static void	get_minishell_pid(t_env *minishell)
{
	char	*tmp;
	char	*buf;
	int		fd;

	fd = open("/proc/self/status", O_RDONLY);
	if (fd < 0)
		return ;
	tmp = get_next_line(fd);
	while (tmp)
	{
		// Check si la contient "Pid:"
		if (ft_strstr(tmp, "Pid:"))
		{
			buf = tmp;
			while (*buf && *buf < '0' && *buf > '9')
				buf++;
			minishell->pid = ft_strndup(buf, ft_strlen(buf));
			break ;
		}
		free(tmp);
		tmp = get_next_line(fd);
	}
	if (tmp)
		free(tmp);
	close(fd);
}

/*
* Charge lemplacement actuelle dans minishell->old_path
*/

static void	init_work_path(t_env *minishell)
{
	char	*tmp;
	size_t	size;

	size = 10;
	tmp = malloc(sizeof(char) * (size + 1));
	if (!tmp)
		return ;
	while (!getcwd(tmp, size))
	{
		free(tmp);
		if (errno == ENOENT)
		{
			printf("shell-init: error retrieving current directory: \
getcwd: cannot access parent directories: No such file or directory\n");
			return ;
		}
		if (errno != ERANGE)
			return ;
		size += 10;
		tmp = malloc(sizeof(char) * (size + 1));
		if (!tmp)
			return ;
	}
	ft_lstadd_back(&minishell->env_list, ft_create_node(create_classic_var("PWD", tmp)));
	minishell->act_path = tmp;
}
/*
	* Recuperre le nom du lutilisateur actuelle depuis env
	* essaye douvrir /etc/passwd pour recupere les ids de luser
	* utils gnl jusqua trouver luser puis split pour recup
	* Luser id et le group id
	* puis les stock dans la structure t_user
	*
	*
	* A LIRE
	* Si split echoue quand il char l'env
	* On arrete minishell ou on continue
	* lexecution sans env ?????????????
*/

static void	load_env_user(t_env *m)
{
	char	**split;
	char	*line;
	int		fd;

	fd = open("/etc/passwd", O_RDONLY);
	if (fd < 0)
		return ;
	line = get_next_line(fd);
	while (line && m->user.uid == 0)
	{
		split = ft_split(line, ':');
		if (!split)
			return ;
		if (ft_strncmp(split[0], m->user.user, ft_strlen(m->user.user)) == 0)
			m->user.uid = ft_atoi(split[2]);
		if (ft_strncmp(split[0], m->user.user, ft_strlen(m->user.user)) == 0)
			m->user.gid = ft_atoi(split[3]);
		free_split(split);
		free(line);
		line = get_next_line(fd);
	}
	free(line);
	close(fd);
}

/*
 * Meme probleme que plus haut
 * Si il manque des info dans
 * l'env continuer lexec ou
 * arreter ?
*/
void	init_env(t_env *minishell)
{
	if (!minishell)
		return ;
	minishell->user.user = getenv("USER");
	minishell->user.home = getenv("HOME");
	minishell->user.uid = 0;
	init_work_path(minishell);
	load_env_user(minishell);
	get_minishell_pid(minishell);
	get_path(minishell);
}
