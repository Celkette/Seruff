/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 09:40:02 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/08/17 07:54:58 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

/********************************
*  	   MaJ struct t_meta 	    *
*                               *
*********************************/
//@MàJ de tous les flags dans la structure meta
// @param c le caractère à vérifier
// @param meta Pointeur vers la structure contenant les flags de séparateurs
// @return 1 si une quote est ouverte, 0 sinon
int	ft_update_flag(char c, t_meta *meta)
{
	flag_space(c, meta);
	flag_quote(c, meta);
	flag_pipe(c, meta);
	flag_redir(c, meta);
	if (c == '$' && (flag_quote(c, meta) == 0 || flag_quote(c, meta) == 2))
		meta->dollar += 1;
	if (flag_space(c, meta) == 0)
		meta->only_space = 0;
	if (meta->single_quote != 0 || meta->double_quote != 0)
		return (1);
	return (0);
}
