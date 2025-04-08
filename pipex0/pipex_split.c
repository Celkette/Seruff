/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:45:26 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/04/01 12:52:25 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	ft_count_word(char const *str, char c)
{
	int	n_word;
	int	in_word;
	int	i;
	int	in_quotes;

	n_word = 0;
	in_word = 0;
	i = 0;
	in_quotes = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			in_quotes = !in_quotes;
		if (str[i] != c && !in_word)
		{
			in_word = 1;
			n_word++;
		}
		else if (str[i] == c && !in_quotes)
		{
			in_word = 0;
		}
		i++;
	}
	return (n_word);
}

static void	*ft_free_all(char **dest, int index)
{
	while (index > 0)
	{
		free(dest[index - 1]);
		index--;
	}
	free(dest);
	return (NULL);
}

static char	*ft_find_start_end_write(char const *str, char c, size_t *i)
{
	int		in_quotes;
	size_t	word_end;
	size_t	word_start;

	in_quotes = 0;
	while (str[*i] == c)
		(*i)++;
	word_start = (*i);
	while (str[*i] != '\0')
	{
		if (str[*i] == '\'')
		{
			in_quotes = !in_quotes;
			if (in_quotes)
				word_start = *i + 1;
		}
		else if (str[*i] == c && !in_quotes)
			break ;
		(*i)++;
	}
	word_end = *i;
	if (word_end > word_start && str[word_end - 1] == '\'' && !in_quotes)
		word_end = word_end - 1;
	return (ft_substr(str, word_start, (word_end - word_start)));
}

char	**pipex_ft_split(char const *s, char c)
{
	int		n_word;
	int		index;
	char	**dest;
	size_t	i;

	n_word = ft_count_word(s, c);
	index = 0;
	i = 0;
	dest = (char **)malloc(sizeof(char *) * (n_word + 1));
	if (!dest)
		return (NULL);
	while (index < n_word)
	{
		dest[index] = ft_find_start_end_write(s, c, &i);
		if (!dest[index])
		{
			ft_free_all(dest, index);
			return (NULL);
		}
		index++;
	}
	dest[index] = NULL;
	return (dest);
}
