/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_util2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 13:57:40 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/07/30 14:40:25 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

/***********************************************
*	MàJ des flag de la structure token   	   *
*		-si precedent est un heredoc <<		   *
*		-si precedent est une redir < > >>	   *
************************************************/
void print_token_flags(t_token *token)
{
    if (!token)
    {
        printf("Token NULL\n");
        return;
    }
    printf("---- TOKEN FLAGS  ----\n");
    //printf("type       = %d\n", token->type);
    printf("data       = %s\n", token->data ? token->data : "(null)");
    //printf("consume    = %d\n", token->consume);
    //printf("quote_type = %d\n", token->quote_type);
    //printf("is_quote   = %d\n", token->is_quote);
    printf("hered      = %d\n", token->hered);
    printf("redir      = %d\n", token->redir);
    printf("---------------------\n");
}
void	update_token_struct(t_list *list)
{
	t_list*current;
	t_token	*token;
    int i;

	if (!list || !list->data)
	return;
	current = list;
	if (list && list->data)
	{
        while (current && current->next && current->next->data)// Si le token suivant existe
		{
            i = 0;
            token = (t_token *)current->data;
			if (ft_strncmp(token->data, "<<", 2) == 0)// Si le token actuel est un heredoc => PAS D"EXPAND
            //token->hered = 1;
            ((t_token *)current->next->data)->hered = 1; // On marque le token suivant comme precedé par heredoc
			else if (ft_strncmp(token->data, "<", 1) == 0) // Si le token actuel est une redir in => PAS DE WORDSPLIT APRES EXPAND
            ((t_token *)current->next->data)->redir = 1; // On marque le token suivant comme precedé par une redir
			else if (ft_strncmp(token->data, ">", 1) == 0) // Si le token actuel est une redir out => PAS DE WORDSPLIT APRES EXPAND
            ((t_token *)current->next->data)->redir = 1; // On marque le token suivant comme precedé par une redir
			else if (ft_strncmp(token->data, ">>", 2) == 0) // Si le token actuel est une redir append => PAS DE WORDSPLIT APRES EXPAND
            ((t_token *)current->next->data)->redir = 1; // On marque le token suivant comme precedé par une redir
			else if (is_in_charset(token->data[i], "0123456789"))
            {
                while (is_in_charset(token->data[i], "0123456789"))
                    i++;
                if (token->data[i]  == '<' && token->data[i + 1]  == '<')
                    ((t_token *)current->next->data)->hered = 1;
            }
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
		if ((new_data[1] == '\"' || new_data[1] == '\'') && meta->quote_open == 0)//si hors quote et suivi de quote -> supprime le dollar
		{
			new_data = delete_dollar(new_data, 0);
			token->data = new_data;
			return (new_data);
		}
		if ((new_data[1] == '\"' || new_data[1] == '\'') && meta->quote_open == 1)//si entre quote garde le dollar
			return (new_data);
	}
	return (new_data);
}
