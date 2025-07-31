/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 10:12:58 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/07/31 10:14:56 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

/****************************************************
* Logique pour  expand la liste chainée token_list  *
*****************************************************/
t_list	*expand_the_list(t_env *minishell, t_list *token_list, t_meta *meta, t_list *var_list)
{
	t_list	*current;
	t_list	*expand_list;
	t_token	*token;
	t_list	*next;
	char	*new_data;
	//printf("ENTRE DANS expand_list()\n");
	
	update_token_struct(token_list);
	current = token_list;
	next = 0;
	while (current)
	{
		//printf("current dans boucle expand_list= %s\n", ((t_token *)current->data)->data);
		//if (current->next)
		//printf("current->next=%s\n", ((t_token *)current->next->data)->data);
		//else
		//printf("current->next=NULL\n");
		next = current->next;
		token = (t_token *)current->data;
		new_data = token->data;
//Si precedent token n'est pas << alors on expand
// if(current->prev && strncmp(token->prev))
		if (token->hered == 1)// Si token precedant == <<(heredoc)
		{
			new_data = manage_hered(current, meta);
		}
		if(token->hered != 1)//Si token non precedé de <<(heredoc) alors on expand
			new_data = expand(minishell, var_list, token->data, meta);
		if (ft_strncmp(token->data, new_data, ft_strlen(token->data)) != 0)
		{
			free(token->data);
			token->data = new_data;
			if(token->data[0] != '\0' && token->redir != 1)//Si token non precedé de < > ou >>(redir) alors on wordsplit(retokenize)
			{
				expand_list = tokenize_the_hell(token->data, " \t");
			//printf("current avant d'entre dans replace_expand= %s\n", ((t_token *)current->data)->data);
			//printf("expand_list avant replace_expand\n");
			//print_token_list(expand_list);
			//printf("cur_list dans expand_list avant delete_node= %p\n", ((t_token *)current->data));
				token_list = replace_expand(token_list, expand_list, current);
			//printf("token_list apres replace_expand\n");
			//print_token_list(token_list);
			}
		}
		//if (token_list == NULL)
		//break;
		current = next;
	}
	//printf("RETURN DE expand_list() = \n");
	//print_token_list(token_list);
	return (token_list);
}

//print_token_list(token_list);
//printf("token->data = [%s]\n", token->data);
//print_token_list(expand_list);
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
/* t_list *list = liste de variable locale) */
char	*expand(t_env *minishell, t_list *var_list, char *str, t_meta *meta)
{
	int		i;
	char	*new_str;
	char	*tmp;
	i = 0;
	if (!str || *str == '\0')
		return (str);
	new_str = str;
	//tmp = str;
	if (is_dollar_in(str, meta) == 0)
		return (ft_strdup(str)); // Si pas de dollar, retourne la chaîne d'origine
	ft_init_meta(meta);
	while (new_str && new_str[i] != '\0')
	{
		tmp = new_str;
	flag_quote(new_str[i], meta);
		if (new_str[i] == '$' && meta->single_quote != 1)
		{
			new_str = check_which_expand(minishell, var_list, new_str, meta, &i);
			if (ft_strncmp(tmp, new_str, ft_strlen(tmp)) != 0)
				i--;
		}
		i++;
	}
//printf("RETURN DE expand() = %s\n", new_str);
	return (ft_strdup(new_str));
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
char	*check_which_expand(t_env *minishell, t_list *list, char *str, t_meta *meta, int *i)
{
	char	*new_str;
	int		res;
//printf("ENTRE DANS check_which_expand()\n");
//printf("check_which_expand -> str = %s\n", str);
	res = is_expand_ok(str, meta, *i);
	if (res == 1) //pas d'expand et garde le $
		new_str = str;
	else if (res == 2) //pas d'expand et supprime le $
	{
		new_str = delete_dollar(str, *i);
		(*i)--;
	}
	else if (res == 0) //expand a faire
		new_str = expand_char(minishell, list, str, meta, *i);
//printf("RETURN DE check_which_expand() = %s\n", new_str);
//printf("check_which_expand -> new_str = %s\n", new_str);
	return (strdup(new_str));
}

/**************************************************
* logique pour expand une variable *
*		*-expand_char()                        *
*			*-appelle find_var_name()		      *
*			 -appelle ft_strndup()			      *
*			*-appelle var_name_index_end()		  *
*			 -appelle ft_strlen()				  *
*			*-appelle is_in_charset()			  *
*			*-appelle special_case()			      *
*			*-appelle find_value_var()			  *
*			 -appelle ft_strjoin()			      *
* Return new_token avec valeur variable remplacée *
***************************************************/
//i = index du $ dans str
char	*expand_char(t_env *minishell, t_list *list, char *str, t_meta *meta, int i)
{
	const char	*str_begin;
	const char	*var_name;
	const char	*var_val;
	const char	*str_end;
	char		*new_str;
//printf("ENTRE DANS expand_char()\n");
//printf("str= %s\n", str);
	var_name = find_var_name(str, meta, i);
//printf("var name= %s\n", var_name);
	str_begin = ft_strndup((const char *)str, i);
//printf("str_begin= %s\n", str_begin);
	str_end = ft_strndup((const char *)(str + var_name_index_end(str, meta)
				+ 1), (ft_strlen(str) - var_name_index_end(str, meta)));
/*printf("var_name_index_end= %d\n", var_name_index_end(str, meta));
printf("str_end= %s\n", str_end);
printf("ft_strlen(str)= %d\n", ft_strlen(str));
printf("(len str - index fin de var_name= %d\n", (ft_strlen(str) - var_name_index_end(str, meta)));
printf("len a partir de index (str + index fin de var_name)= %d\n", ft_strlen(str + var_name_index_end(str, meta)));
*///str_end = ft_strndup((const char *)(str + i + ft_strlen(var_name) + 1), (ft_strlen(str + i + ft_strlen(var_name)))); //ne permet pas de gerer {}
	if (is_in_charset(var_name[0], "$?0123456789" ) == 1)
		var_val = special_case(var_name, minishell);
	else
		var_val = find_value_var(minishell, list, (char *)var_name);
//if (var_name == NULL)
//var_name = "moi";
//printf("var_name  = [%s]\n", var_name );
//printf("str_begin = [%s]\n", str_begin);
//printf("var_val = [%s]\n", var_val);
//printf("str_end = [%s]\n\n", str_end);
	new_str = ft_strjoin(ft_strjoin(str_begin, var_val), str_end);
printf("RETURN DE expand_char() = -%s-\n", new_str);
	return (strdup(new_str));
}

/***************************************************************
*    Logique pour remplacer un élément(cur_list) d'une liste   * 
* chainée(token_list) par une autre liste chainée(expand_list) *
* -> creation d'une nouvelle liste chainée                        *
****************************************************************/
	//printf("cur_list apres delete_node= %p\n", ((t_token *)cur_list->data));			//adresse du pointeur
	//printf("cur_list apres delete_node= %p\n", ((t_token *)cur_list->data)->data);	//valeur du pointeur
t_list	*replace_expand(t_list *list, t_list *expand_list, t_list *cur_list)
{
	t_list	*result;
//printf("ENTRE DANS replace_expand()\n");
//printf("print_token_list EN ENTRANT DANS replace_expand() \n");
//print_token_list(list);
//printf("current avant delete_node= %s\n", ((t_token *)cur_list->data)->data);
	result = ft_delete_node(cur_list);//==pointeur sur/-1 noeud suivant/1 noeud precedent celui supprimé/null si list vide
			//cur_list a été free par delete_node->il ne faut plus l'utiliser 		
//if (!result)
//printf("resultat il es vrai que delete_node=NULL\n");	
	if (expand_list == NULL)
	{
		if (cur_list == list)
			return (result);// retourne le noeud precedent ou suivant
		return (list);//retourne la tete de liste
	}	
	if (!result) //si la liste obtenue apres delete est vide
	{
		list = expand_list; //la liste devient expand_list
	}
	else if (result->del == 1) // on est sur l'element precedent celui supprimé == suppression en milieu de liste
	{
		ft_add_list_at_middle(list, expand_list, result);
	}
	else if (result->del == -1) // on est sur l'element suivant celui supprimé == suppression en tete de liste
	{
		list = result;
		list = ft_add_list_at_begin(list, expand_list);
	}
	return (list);
}
