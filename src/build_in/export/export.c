/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:27:16 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/29 11:27:55 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

void	export_part1_2(int *code, int *i, char *tmp)
{
	// Pour export VAR (sans valeur), on valide juste le nom
	while (tmp[*i])
	{
		if (!ft_isalnum(tmp[*i]) && tmp[*i] != '_' && *code != 2)
			*code = 1;
		(*i)++;
	}
}

char	*export_part1(int *code, char *tmp)
{
	int		i;
	char	*sep;

	if (!tmp || *tmp == 0)
		return (NULL);
	sep = ft_strchr(tmp, '=');
	i = 0;
	if (!ft_isalpha(tmp[i]) && tmp[i] != '_' && *code != 2)
		*code = 1;
	if (!sep && !*code && tmp[i])
		export_part1_2(code, &i, tmp);
	else if (sep)
	{
		i++;
		while (tmp && tmp[i] && &tmp[i] != sep)
		{
			if (!ft_isalnum(tmp[i]) && tmp[i] != '_' && *code != 2)
				*code = 1;
			i++;
		}
	}
	return (sep);
}

int	export_built_in_part2(void *p, t_arg *arg)
{
	t_list	*list_arg;
	int		code;

	code = 0;
	if (!arg->minishell->env_list || !(t_list *)arg->node->arg
		|| !((t_list *)arg->node->arg)->data)
	{
		code = export_no_arg(p, arg->minishell);
		if (((t_arg *)p)->node->top)
			exit_minishell(((t_arg *)p)->minishell);
		return (code);  // PAS de free(p) ici
	}
	list_arg = (t_list *)arg->node->arg;
	while (list_arg)
	{
		export_while(arg, list_arg, &code);  // Toujours continuer
		list_arg = list_arg->next;
	}
	return (code);  // PAS de free(p) ici non plus
}

int	export_built_in(void *p)
{
	t_arg	*arg;

	if (!p)
		return (1);
	arg = (t_arg *)p;
	if (!arg->node || !arg->minishell)
	{
		if (arg->node && ((t_arg *)p)->node->top)
			exit_minishell(((t_arg *)p)->minishell);
		return (1);
	}
	if (arg->node->content && *((t_arg *)p)->node->content == '=')
	{
		return (1);
	}
	return (export_built_in_part2(p, arg));
}
