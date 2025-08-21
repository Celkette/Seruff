/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaporte <nlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:34:04 by nlaporte          #+#    #+#             */
/*   Updated: 2025/07/28 11:50:50 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

t_arg	*get_arg_built_in(t_env *minishell, t_tree *node)
{
	t_arg	*arg;

	if (!minishell || !node)
		return (NULL);
	arg = malloc(sizeof(t_arg));
	if (!arg)
		return (NULL);
	arg->minishell = minishell;
	arg->node = node;
	return (arg);
}
