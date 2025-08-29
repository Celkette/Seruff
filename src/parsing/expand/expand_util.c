/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celine <celine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:37:04 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/08/25 00:26:48 by celine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

/**************************************************
*   Verifie si $ present dans le segment  		  *
*		*is_dollar_in()
*			-appelle ft_init_meta(meta)			      *
***************************************************/
int	is_dollar_in(char *str, t_meta *meta)
{
	int	i;

	i = 0;
	if (!str || !str[i])
		return (0);
	ft_init_meta(meta);
	while (str && str[i])
	{
		flag_quote(str[i], meta);
		if (str[i] == '$' && meta->single_quote != 1)
		{
			meta->dollar += 1;
		}
		i++;
	}
	if (meta->dollar > 0)
		return (1);
	return (0);
}

/*******************************************
* logique pour savoir $ doit etre expand   *
*		*-is_expand_ok()                   *
*			-appelle is_in_charset()	   *
* Return 0 si expand                       *
* Return 1 pas d'expand + garde $          *
* Return 2 pas d'expand + suppression $       *
********************************************/
int	is_expand_ok(char *str, t_meta *meta, int i)
{
	if (str[i] == '$')
	{
		if (str[i + 1] == '\0')
			return (1);
		if (str[i + 1] == ' ' || str[i + 1] == '\t')
			return (1);
		if ((str[i + 1] == '\'' || str[i + 1] == '\"') && meta->quote_open == 0)
			return (2);
		if ((str[i + 1] == '\"' || str[i + 1] == '\'') && meta->quote_open == 1)
			return (1);
		if (is_in_charset(str[i + 1], CHARSET_3) == 1)
			return (0);
	}
	return (1);
}

/**************************************************
* Gestion de l'expand $$ $? $0 $->9               *
*		*-special_case)                           *
*			-appelle ft_strdup()				  *
*			-appelle is_in_charset()			  *
*			-appelle retour PID			     	  *
*			-appelle retour nom shell			  *
*			-appelle retour arg			     	  *
*			-appelle retour derniere cmd			     	  *
* Return sur fonction ou  ""                   *
***************************************************/
char	*special_case(const char *name, t_env *minishell)
{
	if (!name || !minishell)
		return (NULL);
	if (*name == '$')
		return (ft_strndup(minishell->pid, ft_strlen(minishell->pid)));
	else if (*name == '0')
		return (ft_strndup("minishell", 9));
	else if (*name == '?')
	{
		if (g_exit_code == 130)
		{
			minishell->exec.exit_code = 130;
			g_exit_code = 0;
		}
		return (ft_itoa(minishell->exec.exit_code));
	}
	else if (*name >= '1' && *name <= '9')
		return (ft_strdup(""));
	return (NULL);
}

/***********************************************
*	Logique pour supprimer le $ sans expand	   *
*			-appelle ft_strndup()			   *
*			-appelle ft_strjoin()			   *
*			-appelle ft_strlen()			   *
************************************************/
char	*delete_dollar(char *str, int i)
{
	char	*str_begin;
	char	*str_end;
	char	*new_str;
	char	*result;

	str_begin = ft_strndup((const char *)str, i);
	str_end = ft_strndup((const char *)(str + i + 1), (ft_strlen(str) - i -1));
	new_str = ft_strjoin(str_begin, str_end);
	free(str_begin);
	free(str_end);
	result = strdup(new_str);
	free(new_str);
	return (result);
}

/*******************************************************
*  Logique de suppression des token vide (hors quote)  *
*  a faire apres l'expand et avant la suppression des quote
*  si (!next) return le prev/ si (next)return le next/ sinon null
***************************************************/
t_list	*remove_empty_tokens(t_list *list)
{
	t_list	*current;
	t_list	*next;
	t_token	*token;

	current = list;
	while (current)
	{
		next = current->next;
		token = (t_token *)current->data;
		if (token->data[0] == '\0' && token->quote_type == 0)
			list = ft_delete_remove_token(current);
		current = next;
	}
	list = head_of_list(list);
	return (list);
}
