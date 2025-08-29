/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_check_syntaxe.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 09:39:30 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/08/27 14:11:34 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

/************************
*   check_empty_input   *
*************************/
//@Verifie existence d'espace ou tab dans la chaine
// @param str La chaîne de caractères à vérifier
// @return 1 si chaîne est null ou vide (seulement  espaces ou tab), 0 sinon
static int	check_empty_input(char *str)
{
	int	i;

	i = 0;
	if (str == NULL)
		return (1);
	while (str[i])
	{
		if (str[i] != ' ' && str[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}

/********************************
*   check_quote syntaxe error   *
*         unclose quote         *
*********************************/
//@Verifie si quote unique et met à jour les flags quote dans struct meta
// @param str La chaîne de caractères à vérifier
// @param meta Pointeur vers la structure contenant les flags de quotes
// @return 0 si pas d'erreur, 3 si quote non fermée (attend fin de la commande)
static int	check_quotes(char *str, t_meta *meta)
{
	int	i;

	i = 0;
	ft_init_meta(meta);
	while (str[i])
	{
		flag_quote(str[i], meta);
		i++;
	}
	if (meta->single_quote != 0 || meta->double_quote != 0)
	{
		ft_putstr_fd(ERROR_QUOTE, 1);
		return (1);
	}
	return (0);
}

/********************************
*   check_pipe syntaxe error    *
* first/last position ou double *
*********************************/
//@Verifie si syntaxe pipe correcte
// @param str La chaîne de caractères à vérifier
// @param meta Pointeur vers la structure contenant les flags de quotes
// @return 0 si pas d'erreur, 1 sinon et RENVOIE LE PROMPT
static int	check_pipe(char *str, t_meta *meta)
{
	int	i;

	i = 0;
	ft_init_meta(meta);
	while (str[i])
	{
		if (str[i] == '|' && (meta->only_space == 1 || \
			meta->last_char_pipe == 1))
		{
			if (str[i + 1] && str[i + 1] == '|')
				ft_putstr_fd(ERROR_PIPE2, 1);
			else
				ft_putstr_fd(ERROR_PIPE1, 1);
			return (1);
		}
		ft_update_flag(str[i], meta);
		if (str[i + 1] == '\0' && meta->last_char_pipe == 1)
		{
			ft_putstr_fd(ERROR_PIPE1, 1);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	check_pipe2(char *str, t_meta *meta)
{
	int	i;

	i = 0;
	ft_init_meta(meta);
	while (str[i])
	{
		if (str[i] == '|' && str[i - 2] && str[i + 2])
		{
			if ((str[i - 2] == '<' && str[i - 1] == '<' && str[i + 1] == '>'
					&& str[i + 2] == '>') || (str[i - 1] == '<'
					&& str[i + 1] == '>'))
			{
				ft_putstr_fd(ERROR_PIPE1, 1);
				return (1);
			}
		}
		i++;
	}
	return (0);
}

/********************************
*        check complet          *
*                               *
*********************************/

int	check_lexing(char *s, t_meta *meta)
{
	ft_init_meta(meta);
	if (!s || check_empty_input(s))
		return (1);
	if (check_pipe(s, meta))
		return (2);
	if (check_pipe2(s, meta))
		return (4);
	if (check_quotes(s, meta))
		return (3);
	return (0);
}
