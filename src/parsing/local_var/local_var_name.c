/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   local_var_name.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celine <celine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 09:19:32 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/08/24 17:40:11 by celine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

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
	if (!str)
		return (NULL);
	ft_init_meta(meta);
	while (str[j] && j < i)
	{
		j++;
	}
	if (str[j] == '$')
	{
		j++;
		if (is_in_charset(str[j], "$?0123456789") == 1)
			return (ft_strndup(str + j, 1));
		if (is_in_charset(str[j], CHARSET_1) == 1)
			return (manage_name(str + j));
	}
	return (NULL);
}

char	*manage_name(char *str)
{
	int	i;
	int	start;
	int	end;

	if (!str)
		return (NULL);
	i = 0;
	if (is_in_charset(str[i], CHARSET_1) == 1)
	{
		start = i;
		while (str[i] && is_in_charset(str[i], CHARSET_2))
			i++;
		end = i;
		return (ft_strndup(str + start, end - start));
	}
	return (NULL);
}
