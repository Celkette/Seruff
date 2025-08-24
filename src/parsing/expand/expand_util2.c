/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_util2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celine <celine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 13:57:40 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/08/23 15:15:26 by celine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

/***********************************************
*	MàJ des flag de la structure token   	   *
*		-si precedent est un heredoc <<		   *
*		-si precedent est une redir < > >>	   *
************************************************/
static void	set_token_flags(t_token *token, t_token *next_token)
{
	int	i;

	i = 0;
	if (!token || !next_token)
		return ;
	if (ft_strncmp(token->data, "<<", 2) == 0)
	{
		next_token->hered = 1;
		need_expand_heredoc(next_token);
	}
	else if (ft_strncmp(token->data, "<", 1) == 0)
		next_token->redir = 1;
	else if (ft_strncmp(token->data, ">", 1) == 0)
		next_token->redir = 1;
	else if (ft_strncmp(token->data, ">>", 2) == 0)
		next_token->redir = 1;
	else if (is_in_charset(token->data[i], "0123456789"))
	{
		while (is_in_charset(token->data[i], "0123456789"))
			i++;
		if (token->data[i] == '<' && token->data[i + 1] == '<')
			next_token->hered = 1;
	}
}

void	update_token_struct(t_list *list)
{
	t_list	*current;

	if (!list || !list->data)
		return ;
	current = list;
	if (list && list->data)
	{
		while (current && current->next && current->next->data)
		{
			set_token_flags((t_token *)current->data,
				(t_token *)current->next->data);
			current = current->next;
		}
	}
}

char	*manage_hered(t_list *current, t_meta *meta)
{
	t_token	*token;
	char	*new_data;

	token = (t_token *)current->data;
	new_data = token->data;
	if (new_data[0] == '$')
	{
		if ((new_data[1] == '\"' || new_data[1] == '\'')
			&& meta->quote_open == 0)
		{
			new_data = delete_dollar(new_data, 0);
			token->data = new_data;
			return (new_data);
		}
		if ((new_data[1] == '\"' || new_data[1] == '\'')
			&& meta->quote_open == 1)
			return (new_data);
	}
	return (new_data);
}

void	need_expand_heredoc(t_token *token)
{
	int	i;

	i = 0;
	while (token->data[i] != '\0')
	{
		if (token->data[i] == '\'' || token->data[i] == '\"')
		{
			token->is_quote = 1;
			break ;
		}
		i++;
	}
}
