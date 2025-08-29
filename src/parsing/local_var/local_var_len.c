/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   local_var_len.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celine <celine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 09:23:38 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/08/24 17:49:28 by celine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

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
int	var_name_index_end(char *str, t_meta *meta)
{
	int	i;
	int	start;

	i = 0;
	if (!str)
		return (0);
	ft_init_meta(meta);
	while (str[i] && flag_quote(str[i], meta) != 3)
	{
		if (str[i] == '$' && meta->single_quote != 1)
		{
			start = i;
			i++;
			if (is_in_charset(str[i], "$?0123456789" ) == 1)
				return (i);
			if (is_in_charset(str[i], CHARSET_1) == 1)
				return (len_manage_name(str + i) + start);
			else
				return (start);
		}
		i++;
	}
	return (0);
}

int	len_manage_name(char *str)
{
	int	i;
	int	end;

	i = 0;
	if (!str)
		return (0);
	if (is_in_charset(str[i], CHARSET_1) == 1)
	{
		end = 0;
		while (str[i] && is_in_charset(str[i], CHARSET_2))
			i++;
		end = i;
		return (end);
	}
	return (0);
}
