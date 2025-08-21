/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   init_env2.c										:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: cle-rouz <cle-rouz@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2025/08/17 13:03:19 by cle-rouz		  #+#	#+#			 */
/*   Updated: 2025/08/17 13:05:22 by cle-rouz		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "../headers/minishell.h"

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
/*********************************************
* si user ID correspond a celui du shell     *
* recupere et stocke uid et gid dans t_user  *
**********************************************/
static void	set_user_ids_from_passwd(t_env *m, char *line)
{
	char	**split;

	split = ft_split(line, ':');
	if (!split)
		return ;
	if (ft_strncmp(split[0], m->user.user, ft_strlen(m->user.user)) == 0)
		m->user.uid = ft_atoi(split[2]);
	if (ft_strncmp(split[0], m->user.user, ft_strlen(m->user.user)) == 0)
		m->user.gid = ft_atoi(split[3]);
	free_split(split);
}

/*********************************************
* Ajoute user ID dans liste var d'env        *
* UID=                                       *
**********************************************/
static void	add_uid_to_env_list(t_env *m)
{
	t_var	*tmp;
	char	*tmps;

	tmps = ft_itoa(m->user.uid);
	if (!tmps)
		return ;
	tmp = create_classic_var("UID", tmps);
	free(tmps);
	if (!tmp)
		return ;
	ft_lstadd_back(&m->env_list, ft_create_node(tmp));
}

/*********************************************
* charge infos user ds la struct du shell    *
**********************************************/
void	load_env_user(t_env *m)
{
	char	*line;
	int		fd;

	fd = open("/etc/passwd", O_RDONLY);
	if (fd < 0)
		return ;
	line = get_next_line(fd);
	while (line && m->user.uid == 0)
	{
		set_user_ids_from_passwd(m, line);
		free(line);
		line = get_next_line(fd);
	}
	if (m->user.uid != 0)
		add_uid_to_env_list(m);
	free(line);
	close(fd);
}
/*static void	load_env_user(t_env *m)
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
	if (m->user.uid != 0)
	{
		t_var	*tmp;
		char	*tmps;

		tmps = ft_itoa(m->user.uid);
		if (!tmps)
			return ;
		tmp = create_classic_var("UID", tmps);
		free(tmps);
		if (!tmp)
			return ;
		ft_lstadd_back(&m->env_list, ft_create_node(tmp));
	}
	free(line);
	close(fd);
}
*/
