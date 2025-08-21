/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaporte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 15:28:38 by nlaporte          #+#    #+#             */
/*   Updated: 2025/06/15 17:43:19 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

char	*ft_strstr(char *str, char *to_find)
{
	int	i;
	int	j;

	if (!str || !to_find)
		return (0);
	i = 0;
	j = ft_strlen(to_find);
	if (j <= 0)
		return (str);
	while (str[i])
	{
		if (str[i] == to_find[0])
		{
			if (ft_strncmp(&str[i], to_find, j - 1) == 0)
				return (&str[i]);
		}
		i++;
	}
	return (0);
}
