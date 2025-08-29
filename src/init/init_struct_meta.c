/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_struct_meta.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 15:41:14 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/08/17 06:50:39 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

/********************************
*  	   Init struct t_meta 	    *
*      Remise a 0 des flag      *
*********************************/
//@Initialise la structure meta à 0
// @param meta Pointeur vers la structure à initialiser
// @return void
void	ft_init_meta(t_meta *meta)
{
	meta->single_quote = 0;
	meta->double_quote = 0;
	meta->quote_open = 0;
	meta->last_quote_is = '\0';
	meta->pipe = 0;
	meta->last_char_pipe = 0;
	meta->redir_in = 0;
	meta->redir_out = 0;
	meta->dollar = 0;
	meta->space = 0;
	meta->only_space = 1;
}
