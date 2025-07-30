/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celine <celine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 10:08:14 by celine            #+#    #+#             */
/*   Updated: 2025/07/28 08:13:15 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

/*************************************************
* logique pour supprimer les quotes d'une liste  * 
**************************************************/
t_list	*remove_list_quotes(t_list *token_list)
{
	t_list	*current;
	t_token	*token;
	char	*tmp;

	if (token_list == NULL)
		return (token_list);
	current = token_list;
	while (current)
	{
		token = (t_token *)current->data;
		tmp = remove_str_quotes(token->data);
		free(token->data);
		token->data = tmp;
		/*ou
tmp = token->data;//garde le pointeur en memoire pour le free ensuite 
token->data = remove_str_quotes(token->data);
free(tmp);
*/
		current = current->next;
	}
	return (token_list);
}
/*************************************************
* logique pour supprimer les quotes d'une string * 
**************************************************/
char	*remove_str_quotes(char *str)
{
	int		i;
	int		j;
	char	quote;
	char	*new_str;

	i = 0;
	j = 0;
	quote = 0;
	new_str = malloc(sizeof(char) * (ft_strlen(str) + 1));
	while (str && str[i])
	{
		if (!quote && (str[i] == '\'' || str[i] == '\"'))
			quote = str[i];
		else if (quote && (str[i] == quote))
			quote = 0;
		else
		{
			new_str[j] = str[i];
			j++;
		}
		i++;
	}
	new_str[j] = '\0'; // Terminer la nouvelle chaîne
	return (new_str);
}
