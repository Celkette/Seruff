/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 15:42:06 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/08/22 16:11:26 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

int	need_expand_here(t_token *token)
{
	int	i;

	i = 0;
	if (token->hered == 0)//si pas un delimiteur de heredoc
		return (0);	
	while (token->data[i] != '\0')
	{
		if (token->data[i] == '\'' || token->data[i] == '\"')
		{
			token->is_quote = 1;
			return (1);
		}
		i++;
	}
}





char	*expand_here()
{
	if (need_expand_here(token) == 1)
	//on expande
}
