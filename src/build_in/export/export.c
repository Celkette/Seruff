/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:27:16 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/21 12:40:28 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static void	print_export_error(char *s)
{
	if (!s)
		return ;
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(s, 2);
	ft_putendl_fd("': not a valid identifier", 2);
}

void	export_part1_2(int *code, int *i, t_arg *arg, char *tmp)
{
	(*i)++;
	while (tmp[*i])
	{
		if (!ft_isalnum(tmp[*i]) && tmp[*i] != '_' && *code != 2)
			*code = 1;
		(*i)++;
	}
	if (((t_list *)arg->node->arg)->next && *code != 2)
	{
		print_export_error(((t_list *)arg->node->arg)->next->data);
		*code = 2;
	}
}

char	*export_part1(int *code, char *tmp, t_arg *arg)
{
	int		i;
	char	*sep;

	sep = ft_strchr(tmp, '=');
	i = 0;
	if (!ft_isalpha(tmp[i]) && tmp[i] != '_' && *code != 2)
		*code = 1;
	if (!sep && !*code && tmp[i])
		export_part1_2(code, &i, arg, tmp);
	i++;
	while (tmp[i] && &tmp[i] != sep)
	{
		if (!ft_isalnum(tmp[i]) && tmp[i] != '_' && *code != 2)
			*code = 1;
		i++;
	}
	return (sep);
}

int	export_while(t_arg *arg, t_list	*list_arg, int *code, void *p)
{
	char	*sep;
	char	*tmp;
	t_var	*var;

	tmp = (char *)list_arg->data;
	if (tmp[0] == 0 || (!ft_isalpha(tmp[0]) && tmp[0] != '_'))
	{
		print_export_error(tmp);
		*code = 2;
	}
	else
		sep = export_part1(code, tmp, arg);
	if (*code == 1)
		(print_export_error(tmp), *code = 0);
	else
	{
		var = create_var(tmp, sep);
		if (!var)
			return ((free(p), -1));
		safe_add_env_list(arg->minishell->env_list, var);
	}
	*code = 0;
	return (0);
}

int	export_built_in(void *p)
{
	t_arg	*arg;
	int		code;
	t_list	*list_arg;

	if (!p)
		return (-1);
	code = 0;
	arg = (t_arg *)p;
	if (!arg->node || !arg->minishell)
	{
		free(p);
		return (-1);
	}
	if (!arg->minishell->env_list || !(t_list *)arg->node->arg
		|| !((t_list *)arg->node->arg)->data)
		return ((code = export_no_arg(arg->minishell), free(p), code));
	list_arg = (t_list *)arg->node->arg;
	while (list_arg)
	{
		if (export_while(arg, list_arg, &code, p) == -1)
			return (-1);
		list_arg = list_arg->next;
	}
	return (free(p), code);
}
