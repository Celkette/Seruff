/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   local_var_len.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 09:23:38 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/07/23 09:57:08 by cle-rouz         ###   ########.fr       */
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
* logique pour recuperer index fin d'une variable $   *
* necessaire pour supprimer les {} 				      *
*		*-var_name_index_end() 		     		      *
*			-appelle ft_init_meta()				      *
*			-appelle flag_quote()				      *
*			-appelle is_in_charset()			      *
*		   *-len_manage_name() gere si $a->z          *
*				-appelle is_in_charset()		      *
*		   *-len_manage_brace() gere si ${..          *
* return index du dernier char de var_name            *
*******************************************************/
int	var_name_index_end(char *s, t_meta *meta)
{
	int	i;
	int	start;

	i = 0;
//	if (!s)
//		return (0);
	ft_init_meta(meta);
	while (s[i] && flag_quote(s[i], meta) != 3)
	{
		//flag_quote(s[i], meta);
		if (s[i] == '$' && meta->single_quote != 1)
		{
			start = i;//index debut du nom de la variable ($compris)
			i++;//on passe le $
			if (is_in_charset(s[i], "$?0123456789" ) == 1)
				return (i);//index fin du nom de la variable
			if (is_in_charset(s[i], get_alpha_char()) == 1)
				return (len_manage_name(s + i) + start);
			if (s[i] == '{' && is_in_charset(s[i + 1], "$?0123456789" ) == 1) // Cas ${VAR}
				return (i + 2);
			if (s[i] == '{' && is_in_charset(s[i + 1], get_alpha_char()) == 1)
				return (len_manage_brace(s + i) + start);// On saute le {
			else
				return (start);
		}
		i++;
	}
	return (0); // Pas de variable trouvée	
}

int	len_manage_name(char *str)
{
	int	i;
	int	end;

	i = 0;
	if (is_in_charset(str[i], get_alpha_char()) == 1)
	{
		while (str[i] && is_in_charset(str[i], get_alphanum_charset()))
			i++;
		end = i; // Dernier char du nom de la variable
		return (end); //retourne index du dernier char de var_name
	}
	return (0);
}

int	len_manage_brace(char *str)
{
	int	i;
	int	end;

	i = 0;
	if (str[i] == '{')
	{
		while (str[i] && str[i] != '}')
			i++;
		end = i + 1;
		return (end);
	}
	return (0);
}

/********************************************************
* logique pour recuperer index debut d'une variable $   *
*		var_name_index_start                            *
*			-appelle ft_init_meta()				   	    *
*			-appelle flag_quote()				  	    * 							*
*********************************************************/
/*n'est plus necessaire
int	var_name_index_start(char *str, t_meta *meta)
{
	int i;

	i = 0;
	ft_init_meta(meta);
	while(str && str[i])
	{
		flag_quote(str[i], meta);
		if (str[i] == '$' && meta->single_quote != 1)
			return (i);
		i++;
	}
	return (-1); // Pas de variable trouvée
}*/