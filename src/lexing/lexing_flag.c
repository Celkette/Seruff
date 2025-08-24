/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_flag.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celine <celine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 09:39:48 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/08/23 11:21:06 by celine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

//@MàJ  du flag espace dans la structure meta
// @param c le caractère à vérifier
// @param meta Pointeur vers la structure contenant le flag d'espace    
// @return 1 si un espace ou une tabulation est trouvé, 0 sinon
int	flag_space(char c, t_meta *meta)
{
	if (c == ' ' || c == '\t')
	{
		meta->space += 1;
		return (1);
	}
	return (0);
}

//@MàJ  des flags quote dans la structure meta 
// @param s le caractère à vérifier
// @param meta Pointeur vers la structure contenant les flags de quotes
// @return 0 si quote fermée, 1 si quote ' ouverte, 2 si quote " ouverte
int	flag_quote(char c, t_meta *meta)
{
	if (c == '\'' || c == '\"')
	{
		if (meta->quote_open == 0)
		{
			meta->quote_open = !meta->quote_open;
			meta->last_quote_is = c;
		}
		else if (meta->quote_open == 1 && meta->last_quote_is == c)
		{
			meta->quote_open = !meta->quote_open;
			meta->last_quote_is = 0;
		}
		if (c == '\'' && meta->double_quote == 0)
			meta->single_quote = !meta->single_quote;
		else if (c == '\"' && meta->single_quote == 0)
			meta->double_quote = !meta->double_quote;
		if (meta->single_quote != 0)
			return (1);
		if (meta->double_quote != 0)
			return (2);
	}
	return (0);
}

//@MàJ des flags de pipe dans la structure meta
// @param c le caractère à vérifier
// @param meta Pointeur vers la structure contenant les flags de pipe
// @return 1 si un pipe est trouvé, 0 sinon
int	flag_pipe(char c, t_meta *meta)
{
	if (c == '|' && flag_quote(c, meta) == 0)
	{
		meta->pipe += 1;
		meta->last_char_pipe = 1;
		meta->only_space = 0;
		return (1);
	}
	else if (c != '|' && flag_quote(c, meta) == 0 && flag_space(c, meta) == 0)
	{
		meta->pipe = 0;
		meta->last_char_pipe = 0;
		meta->only_space = 0;
	}
	return (0);
}

//@MàJ  des flags de redirection dans la structure meta
// @param c le caractère à vérifier
// @param meta Pointeur vers la structure contenant les flags de redirection
// @return 0
int	flag_redir(char c, t_meta *meta)
{
	if ((c == '<' || c == '>') && flag_quote(c, meta) == 0)
	{
		if (c == '<')
		{
			meta->redir_in += 1;
			meta->only_space = 0;
		}
		else if (c == '>')
		{
			meta->redir_out += 1;
			meta->only_space = 0;
		}
	}
	else if (flag_space(c, meta) == 0)
	{
		meta->redir_in = 0;
		meta->redir_out = 0;
		meta->only_space = 0;
	}
	return (0);
}

//@MàJ du flag dollar dans la structure meta
// @param c le caractère à vérifier
// @param meta Pointeur vers la structure contenant le flag dollar
// @return 0
int	flag_dollar(char c, t_meta *meta)
{
	if (c == '$')
	{
		meta->dollar += 1;
		meta->only_space = 0;
	}
	else if (flag_space(c, meta) == 0)
	{
		meta->only_space = 0;
	}
	return (0);
}
