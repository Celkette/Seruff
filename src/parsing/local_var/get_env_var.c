/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 10:13:05 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/21 12:57:55 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

//@Création de variables d'environnement
// @param str=chaîne de char contenant la var d'env (ex: "PATH=/usr/bin:/bin")
// @return Pointeur vers la structure t_var si l'allocation réussit, NULL
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

//@Creat struct t_var pour var d'env à partir d'1 chaîne au format "KEY=VALUE".
// @param str La chaîne de char contenant la var d'env(ex:"PATH=/usr/bin:/bin")
// @return Pointeur vers la structure t_var si l'allocation réussit, NULL
static t_var	*create_env_var(char *str, char *sep)
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

//@Libère la mémoire allouée pour une liste de variables d'environnement
// @param lst Pointeur vers la liste de variables d'environnement
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

//Copie l'environnement système dans la structure interne du minishell
//@Copie les var d'env du tableau minishell->env dans minishell->env_list
// @param minishell Pointeur vers la structure d'environnement du shell
void	copy_env_var(t_env *minishell)
{
	char	*sep;
	char	**env;
	t_var	*var;

	if (!minishell || !minishell->env)
		return ;
	env = minishell->env;
	while (*minishell->env)
	{
		sep = ft_strchr((const char *)*minishell->env, '=');
		if (sep)
		{
			var = create_env_var(*minishell->env, sep);
			if (!var)
			{
				clear_env_var(minishell->env_list);
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

//@Vérifie si une var d'env existe dans la liste des variables d'environnement
// @param minishell Pointeur vers la structure d'environnement du shell
// @param var_name Le nom de la variable d'environnement à rechercher
// @return 1 si la variable existe, 0 sinon
int	is_var_in_env(t_env *minishell, char *var_name)
{
	t_list	*lst;
	t_var	*var;
	int		len;

	len = ft_strlen(var_name);
	if (!minishell || !minishell->env_list || !var_name)
		return (0);
	lst = minishell->env_list;
	while (lst)
	{
		var = (t_var *)lst->data;
		if (ft_strncmp(var->key, var_name, len) == 0
			&& ft_strlen(var->key) == len)
			return (1);
		lst = lst->next;
	}
	return (0);
}
