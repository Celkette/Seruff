/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaporte <nlaporte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 20:55:43 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/14 07:50:59 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static size_t	ft_next(char *str, char c)
{
	size_t	j;

	j = 0;
	if (*str == c)
	{
		while (*str)
		{
			if (*str++ == c)
				j++;
			else
				break ;
		}
	}
	else
	{
		while (*str)
		{
			if (*str++ != c)
				j++;
			else
				break ;
		}
	}
	return (j);
}

static char	**ft_abort(char **split, int size)
{
	if (!split)
		return (NULL);
	while (size >= 0)
		free(split[size--]);
	free(split);
	return (NULL);
}

static size_t	ft_split_size(char *str, char c)
{
	unsigned int	i;
	size_t			size;

	i = 0;
	size = 0;
	while (str[i])
	{
		if (str[i] == c)
			i += ft_next(&str[i], c);
		else
		{
			i += ft_next(&str[i], c);
			size++;
		}
	}
	return (size);
}

void	free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
		free(split[i++]);
	free(split);
}

char	**ft_split(char *str, char c)
{
	char	**split;
	int		k;

	k = 0;
	if (!str)
		return (NULL);
	split = malloc(sizeof(char *) * (ft_split_size(str, c) + 1));
	if (split == NULL)
		return (NULL);
	while (*str)
	{
		if (*str == c)
			str++;
		else
		{
			split[k] = ft_strndup(str, ft_next(str, c));
			if (split[k++] == NULL)
				return (ft_abort(split, k - 1));
			str += ft_next(str, c);
		}
	}
	split[k] = 0;
	return (split);
}
