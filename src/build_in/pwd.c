/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaporte <nlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 13:29:51 by nlaporte          #+#    #+#             */
/*   Updated: 2025/07/28 12:43:26 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

/*
	* utilise getcwd pour recuperer le repertoir de travail
	* si getcwd echoue avec le code ENOENT, le repertoire 
	* de travail a ete supprime donc on affiche lerreur
	* si EACCESS Impossible de lire ou de parcourir un 
	* composant du chemin d'accès, mais impossibe a 
	* provoquer ??????????
	*
	* doute sur lerreur recuperer depuis bash --posix 5.2
	* a verifier sur les pc 42
	*
	* si getcwd reussi ecris le repertoire de travail dans
	* buf puis on le print avec printf
	*
	* PS: les autres erreur de getcwd sont liee au buf
	*
	* Doc:
	* http://manpagesfr.free.fr/man/man3/getcwd.3.html
*/

int	pwd(void *p)
{
	char	buf[102400];

	(void)p;
	if (!getcwd(buf, 102400))
	{
		if (errno == ENOENT)
		{
			ft_putstr_fd("pwd: error retrieving current directory: getcwd: cannot \
access parent directories: No such file or directory\n", 2);
			return (ENOENT);
		}
	}
	printf("%s\n", buf);
	return (0);
}
