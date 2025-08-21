/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   local_var_value.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 09:25:36 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/08/17 11:59:39 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

/**************************************************
* logique pour recuperer la valeur d'une variable *
*		*-find_value_var()                        *
*			-appelle ft_strdup()				  *
*			-appelle is_var_in_local()			  *
*			-appelle ft_strlen()				  *
*			-appelle ft_strncmp()				  *
*			-appelle getenv()			     	  *
* Return valeur de la var ou ""                   *
***************************************************/

char	*find_value_var(t_env *minishell, char *var_name)
{
	if (!var_name)
		return (NULL);
	if (is_var_in_env(minishell, var_name))
		return (ft_strdup(var_env_value(minishell, var_name)));
	if (is_var_in_local(minishell, var_name))
		return (ft_strdup(var_local_value(minishell, var_name)));
	return (ft_strdup(""));
}
