/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 10:45:38 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/08/20 10:23:51 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

/**************************************************
* logique pour expand une variable *
*		*-expand_char()                        *
*			*-appelle find_var_name()		      *
*			 -appelle ft_strndup()			      *
*			*-appelle var_name_index_end()		  *
*			 -appelle ft_strlen()				  *
*			*-appelle is_in_charset()			  *
*			*-appelle special_case()			      *
*			*-appelle find_value_var()			  *
*			 -appelle ft_strjoin()			      *
* Return new_token avec valeur variable remplacée *
***************************************************/
char	*expand_char(t_env *minishell, char *str, t_meta *meta, int i)
{
	const char	*str_begin;
	const char	*var_name;
	const char	*var_val;
	const char	*str_end;
	char		*new_str;

	if (!str)
		return (NULL);
	var_name = find_var_name(str, meta, i);
	str_begin = ft_strndup((const char *)str, i);
	str_end = ft_strndup((const char *)(str + var_name_index_end(str, meta)
				+ 1), (ft_strlen(str) - var_name_index_end(str, meta)));
	if (is_in_charset(var_name[0], "$?0" ) == 1)
		var_val = special_case(var_name, minishell);
	else
		var_val = find_value_var(minishell, (char *)var_name);
	free((char *)var_name);
	var_name = ft_strjoin(str_begin, var_val);
	new_str = ft_strjoin(var_name, str_end);
	free((char *)str_begin);
	free((char *)str_end);
	free((char *)var_val);
	free((char *)var_name);
	return (new_str);
}

/***************************************************************
*    Logique pour remplacer un élément(cur_list) d'une liste   * 
* chainée(token_list) par une autre liste chainée(expand_list) *
* -> creation d'une nouvelle liste chainée                        *
****************************************************************/
t_list	*replace_expand(t_list *list, t_list *expand_list, t_list *cur_list)
{
	t_list	*result;

	if (!list || !cur_list)
		return (list);
	result = ft_delete_node(cur_list);
	if (expand_list == NULL)
	{
		if (cur_list == list)
			return (result);
		return (list);
	}	
	if (!result)
	{
		list = expand_list;
	}
	else if (result->del == 1)
	{
		ft_add_list_at_middle(list, expand_list, result);
	}
	else if (result->del == -1)
	{
		list = result;
		list = ft_add_list_at_begin(list, expand_list);
	}
	return (list);
}
