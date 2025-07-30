/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:37:04 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/07/30 13:58:04 by cle-rouz         ###   ########.fr       */
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
//printf("flag_quote= %d\n", meta->single_quote);
		if (str[i] == '$' && meta->single_quote != 1)
		{
			meta->dollar += 1;//! si $$ compte 2 dollars
		}
		i++;
	}
//printf("nb $= %d\n", meta->dollar);
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
* Return 2 si expand + suppression $       *
********************************************/
int	is_expand_ok(char *str, t_meta *meta, int i)
{
	if (str[i] == '$')
	{
		//int res = is_in_charset(str[i + 1], "{_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
		//cas pas d'expand
		if (str[i + 1] == '\0')//$ == fin de segment -> garde le dollar
			return (1);
		if (str[i + 1] == ' ' || str[i + 1] == '\t')//si $ suivi d'un space -> garde le dollar
			return (1);
		if ((str[i + 1] == '\"' || str[i + 1] == '\'') && meta->quote_open == 0)//si hors quote et suivi de quote -> supprime le dollar
			return (2);	
		if ((str[i + 1] == '\"' || str[i + 1] == '\'') && meta->quote_open == 1)//si entre quote garde le dollar
			return (1);
		if (is_in_charset(str[i + 1], "{_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789") == 0)// si autre char -> garde le dollar
		//if (res == 0)
			return (1);
	}
	return (0);//si autre cas on expand
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
//static char	*special_case(t_env *minishell, char *name)
char	*special_case(const char *name, t_env *minishell)
{
	if (!name || !minishell)
		return (NULL);
	if (*name == '$')
		return (minishell->pid);
	else if (*name == '0')
		return ("minishell");
	else if (*name == '?')
		return (ft_itoa(minishell->exec.exit_code));
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

	str_begin = ft_strndup((const char *)str, i);
	str_end = ft_strndup((const char *)(str + i + 1), (ft_strlen(str) - i -1));
	new_str = ft_strjoin(str_begin, str_end);
	free(str_begin);
	free(str_end);
//printf("str_begin = [%s]\n", str_begin);
//printf("str_end = [%s]\n\n", str_end);
	return (strdup(new_str));
}

/**************************************************
*  Logique de verification de presence de space   *
* return 1 si oui, 0 si non
***************************************************/
//PAS UTILE
/*int check_space(char *str)
{
	int i;

	i = 0;
	while(str && str[i])
	{
		if (is_in_charset(str[i], " \t") == 1)//si space
			return (1);
	}
	return (0);//si pas de space
}*/
