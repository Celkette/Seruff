/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 10:13:05 by nlaporte          #+#    #+#             */
/*   Updated: 2025/07/31 10:09:05 by nlaporte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

t_var	*create_classic_var(char *key, char *val)
{
	t_var	*var;

	if (!key || !val)
		return (NULL);
	var = malloc(sizeof(t_var));
	if (!var)
		return (NULL);
	var->key = ft_strndup(key, ft_strlen(key));
	if (!var->key)
	{
		free(var);
		return (NULL);
	}
	var->val = ft_strndup(val, ft_strlen(val));
	if (!var->val)
	{
		free(var->key);
		free(var);
		return (NULL);
	}
	return (var);
}

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

void	clear_env_var(t_list *lst)
{
	t_list	*tmp;
	t_var	*var;

	while (lst)
	{
		var = (t_var *)lst->data;
		free(var->key);
		free(var->val);
		free(var);
		tmp = lst->next;
		free(lst);
		lst = tmp;
	}
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
		return ;
	else
	{
		env = minishell->env;
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
	t_list	*lst;
	t_var	*var;
    size_t len = ft_strlen(var_name);

	if (!minishell || !minishell->env_list || !var_name)
		return (0);
	lst = minishell->env_list;
    while (lst)
    {
		var = (t_var *)lst->data;
        // Cherche une variable de la forme "VAR=value"
        if (ft_strncmp(var->key, var_name, len) == 0)
            return (1); // var trouvée
        i++;
		lst = lst->next;
    }
    return (0); // Non trouvée
}

char	*var_env_value(t_env *minishell, char *var_name)
{
	int i = 0;
	t_list	*lst;
	t_var	*var;
    size_t len = ft_strlen(var_name);

	if (!minishell || !minishell->env_list || !var_name)
		return (0);
	lst = minishell->env_list;
    while (lst)
    {
		var = (t_var *)lst->data;
        // Cherche une variable de la forme "VAR=value"
        if (ft_strncmp(var->key, var_name, len) == 0)
            return (var->key); // var trouvée
        i++;
		lst = lst->next;
    }
    return (NULL); // Non trouvée
}
