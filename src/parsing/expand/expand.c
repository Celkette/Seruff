/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 10:12:58 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/08/29 12:09:21 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

/****************************************************
* Logique pour  expand la liste chainée token_list  *
*****************************************************/
static void	handle_expand_list(t_env *minishell,
		t_list *current, t_meta *meta)//, t_list **token_list)
{
	t_token	*token;
	char	*new_data;
	//t_list	*expand_list;

	if (!current || !current->data)
		return ;
	token = (t_token *)current->data;
	new_data = token->data;
	if (token->hered == 1)
		new_data = manage_hered(current, meta);
	else if (token->hered != 1)
		new_data = expand(minishell, token->data, meta);
	if (ft_strncmp(token->data, new_data, ft_strlen(token->data)) != 0)
	{
		free(token->data);
		token->data = new_data;
		// POSIX: Ne pas retokeniser après expansion pour éviter double expansion
		// if (token->data[0] != '\0' && token->redir != 1)
		// {
		//	expand_list = tokenize_the_hell(token->data, " \t");
		//	*token_list = replace_expand(*token_list, expand_list, current);
		// }
	}
	else if (new_data != token->data)
		free(new_data);
}

t_list	*expand_the_list(t_env *minishell, t_list *token_list, t_meta *meta)
{
	t_list	*current;
	t_list	*next;

	update_token_struct(token_list);
	current = token_list;
	next = 0;
	while (current)
	{
		next = current->next;
		handle_expand_list(minishell, current, meta);//, &token_list);
		current = next;
	}
	token_list = remove_empty_tokens(token_list);
	return (token_list);
}

/***************************************************
* Logique pour expand le token qqlsoit le nb de $  *
*		-expand()                                 *
*			-appelle is_dollar_in()		           *
*			-appelle ft_strdup()			       *
*			-appelle ft_init_meta()		           *
*			-appelle flag_quote()				   *
*		   *-check_which_expand                    *
*				-appelle is_expand_ok()	     	   *
*				-appelle delete_dollar()			   *
*				-appelle expand_char()			   *
* Return new_token final toutes variable remplacée *
****************************************************/
static char	*handle_expand(t_env *minishell, char *str, t_meta *meta)
{
	int		i;
	char	*new_str;
	char	*tmp;

	i = 0;
	new_str = str;
	ft_init_meta(meta);
	while (new_str && new_str[i] != '\0')
	{
		tmp = new_str;
		flag_quote(new_str[i], meta);
		if (new_str[i] == '$' && meta->single_quote != 1)
		{
			new_str = check_which_expand(minishell, new_str, meta, &i);
			if (ft_strncmp(tmp, new_str, ft_strlen(tmp)) != 0)
				i--;
			if (i < -1)
				i = -1;
			if (tmp != str && tmp != new_str)
				free(tmp);
		}
		i++;
	}
	return (new_str);
}

char	*expand(t_env *minishell, char *str, t_meta *meta)
{
	if (!str || *str == '\0')
		return (str);
	if (is_dollar_in(str, meta) == 0)
		return (ft_strdup(str));
	return (handle_expand(minishell, str, meta));
}

//printf("expand -> str = %s\n", str);

/***************************************************************************
* Logique pour si expand ou si                         *
*		*-is_expand_ok()                               					   *
*		*-delete_dollar()		         									   *
*		*-expand_char()			     									   *
* Return new_str                         				                   *
*																		   *
* (*i pour prendre l'adresse du i de expand() et pouvoir modifier la       *
* valeur a l'adresse. Cela permet de passer le chgt de valeur a expand() ) *
****************************************************************************/
char	*check_which_expand(t_env *minishell, char *str, t_meta *meta, int *i)
{
	char	*new_str;
	int		res;

	new_str = NULL;
	res = is_expand_ok(str, meta, *i);
	if (res == 1)
		new_str = str;
	else if (res == 2)
	{
		new_str = delete_dollar(str, *i);
		(*i)--;
	}
	else if (res == 0)
		new_str = expand_char(minishell, str, meta, *i);
	return (new_str);
}
