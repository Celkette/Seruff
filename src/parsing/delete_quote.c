/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 10:08:14 by celine            #+#    #+#             */
/*   Updated: 2025/08/27 14:20:57 by cle-rouz         ###   ########.fr       */
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
		if (strcmp(tmp, token->data) != 0)
		{
			free(token->data);
			token->data = tmp;
		}
		else if (strcmp(tmp, token->data) == 0)
			free (tmp);
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
	new_str = str;
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
	new_str[j] = '\0';
	return (ft_strdup(new_str));
}
