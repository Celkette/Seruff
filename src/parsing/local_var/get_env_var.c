/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 10:13:05 by nlaporte          #+#    #+#             */
/*   Updated: 2025/07/29 13:05:08 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static t_var	*create_var(char *str, char *sep)
{
	t_var	*var;
	int		len;

	if (!str || !sep)
		return (NULL);
	var = malloc(sizeof(t_var));
	if (!var)
		return (NULL);
	var->key = malloc(sizeof(char) * (sep - str + 1));
	if (!var->key)
		return (NULL);
	ft_memcpy(var->key, str, (sep - str) * sizeof(char));
	var->key[sep - str] = 0;
	len = ft_strlen(sep) - 1;
	if (len <= 0)
		var->val = NULL;
	else
	{
		var->val = malloc(sizeof(char) * len + 1);
		if (!var->val)
			return (NULL);
		ft_memcpy(var->val, sep + 1, len * sizeof(char));
		var->val[len] = 0;
	}
	return (var);
}

void	printlst(t_list *lst)
{
	t_var *tmp;

	while (lst)
	{
		tmp = (t_var *)lst->data;
		printf("%s=%s\n", tmp->key, tmp->val);
		lst = lst->next;
	}
}

void	copy_env_var(t_env *minishell)
{
	t_var	*var;
	char	*sep;
	char	**env;

	if (!minishell)
		return ;
	if (!minishell->env)
		minishell->env_list = NULL;
	else
	{
		env = minishell->env;
		minishell->env_list = NULL;
		while (*minishell->env)
		{
			sep = ft_strchr((const char *)*minishell->env, '=');
			if (sep)
			{
				var = create_var(*minishell->env, sep);
				if (!var)
				{
					// a voir + free
					minishell->env_list = NULL;
					minishell->env = env;
					return ;
				}
				ft_lstadd_back(&minishell->env_list, ft_create_node(var));
			}
			minishell->env++;
		}
		minishell->env = env;
	}
}

int	is_var_in_env(t_env *minishell, char *var_name)
{
	int i = 0;
    size_t len = ft_strlen(var_name);

    if (!minishell || !minishell->env || !var_name)
        return (0);

    while (minishell->env[i])
    {
        // Cherche une variable de la forme "VAR=value"
        if (ft_strncmp(minishell->env[i], var_name, len) == 0
            && minishell->env[i][len] == '=')
            return (1); // var trouvée
        i++;
    }
    return (0); // Non trouvée
}
char	*var_env_value(t_env *minishell, char *var_name)
{
	int i = 0;
    size_t len = ft_strlen(var_name);

    if (!minishell || !minishell->env || !var_name)
        return (NULL);

    while (minishell->env[i])
    {
        // Cherche une variable de la forme "VAR=value"
        if (ft_strncmp(minishell->env[i], var_name, len) == 0
            && minishell->env[i][len] == '=')
            return (&minishell->env[i][len + 1]); // var trouvée
        i++;
    }
    return (NULL); // Non trouvée
}