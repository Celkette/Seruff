#include "../headers/minishell.h"

static void	manage_error_exit2(void *p, char *str)
{
	if (!str)
	{
		ft_putendl_fd("minishell: exit:  : numeric argument required", 1);
		exit_minishell(((t_arg *)p)->minishell);
		free(p);
		exit(2);
	}
	ft_putstr_fd("minishell: exit: ", 1);
	ft_putstr_fd(str, 1);
	ft_putendl_fd(": numeric argument required", 1);
	exit_minishell(((t_arg *)p)->minishell);
	free(p);
	exit(2);
}

int	manage_error_exit(void *p, t_list *arg)
{
	char	*str;
	int		i;
	int		val;

	if (!arg || !arg->data)
		return (0);
	str = arg->data;
	val = ft_atoi(str);
	if (val < 0)
		val = (256 + val) % 256;
	i = 0;
	if (!str || !*str || ft_strlen(str) > 9)
		manage_error_exit2(p, str);
	if (str[i] == '-')
		i++;
	while (str[i] && str[i] >= '0' && str[i] <= '9')
		i++;
	if (str[i] != 0)
		manage_error_exit2(p, arg->data);
	if (arg->next)
		return (ft_putendl_fd("minishell: exit: too many argument", 1), 1);
	exit_minishell(((t_arg *)p)->minishell);
	free(p);
	exit(val % 256);
}
