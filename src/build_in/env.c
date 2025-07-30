/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaporte <nlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 20:43:35 by nlaporte          #+#    #+#             */
/*   Updated: 2025/07/28 12:37:38 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

int env(void *p)
{
	t_env	*minishell;
	int		i;

	if (!p)
		return (-1);
	minishell = ((t_arg *)p)->minishell;
	if (!minishell->env)
		return (-1);
	i = 0;
	while (minishell->env[i])
		printf("%s\n", minishell->env[i++]);
	return (0);
}
