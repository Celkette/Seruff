/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   local_var_name.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 09:19:32 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/07/23 09:54:23 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static char	*get_alpha_char(void)
{
	return ("_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
}

static char	*get_alphanum_charset(void)
{
	return ("_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
}

/******************************************************
*   logique pour recuperer la nom d'une variable 	  *
*		*find_var_name()                              *
*			-appelle ft_init_meta()				      *
*			-appelle flag_quote()				      *
*			-appelle is_in_charset()			      *
*			-appelle ft_strndup()			          *
*		   *-manage_name() gere si $a->z              *
*				-appelle is_in_charset()		      *
*				-appelle ft_strndup()			      *
*		   *-manage_brace() gere si ${..              *
*				-appelle ft_strndup()			      *
* Return nom de la variable sans $ ni {}              *
*******************************************************/
char	*find_var_name(char *str, t_meta *meta, int i)
{
	int	j;

	j = 0;
	ft_init_meta(meta);
	while (str[j] && j < i)
	{
		//flag_quote(str[j], meta);
		j++;
	}
	if (str[j] == '$') // && meta->single_quote != 1)
	{
		j++;//on passe le $
		if (is_in_charset(str[j], "$?0123456789") == 1)
			//return (ft_strndup(&str[i], 1));//strndup attend un pointeur sur char et pas un char
			return (ft_strndup(str + j, 1));// Cas spécial pour $, ?, 0 -> 9
		if (is_in_charset(str[j], get_alpha_char()) == 1)
			return (manage_name(str + j));
		if (str[j] == '{' && is_in_charset(str[j + 1], "$?0123456789" ) == 1) // Cas ${VAR}
			return (ft_strndup(str + j + 1, 1));// On saute le {		
		if (str[j] == '{' && is_in_charset(str[j + 1], get_alpha_char()) == 1)
			return (manage_brace(str + j));// On saute le {
	}
	return (NULL); // Pas de variable trouvée ERREUR
}

char	*manage_name(char *str)
{
	int	i;
	int	start;
	int	end;

	i = 0;
	if (is_in_charset(str[i], get_alpha_char()) == 1)
	{
		start = i;
		while (str[i] && is_in_charset(str[i], get_alphanum_charset()))
			i++;
		end = i; // Fin du nom de la variable
		return (ft_strndup(str + start, end - start)); // Retourne le nom de la variable
	}
	return (NULL);
}

char	*manage_brace(char *str)
{
	int	i;
	int	start;
	int	end;

	i = 0;
	if (str[i] == '{')
	{
		start = i + 1;
		while (str[i] && str[i] != '}')
			i++;
		end = i;
		if (str[i] == '}')
			return (ft_strndup(str + start, end - start));
	}
	return (NULL);
}
