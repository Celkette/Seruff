#include "push_swap.h"

int check_max_min(char *n)
{
    if (ft_atol(n) > INT_MAX || ft_atol(n) < INT_MIN)
        return (1);
    return (0);
}

int check_double_if_string_arg(char **array)
{
	int i;
	int j;

	i = 0;
	while (array[i] != NULL)
	{
		j = i + 1;
		while (array[j] != NULL)
		{
			if (ft_atol(array[i]) - ft_atol(array[j]) == 0)
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

int check_nb(int argc, char **argv)
{
    int i;
	int j;

    i = 1;
    while (i < argc)
    {
        if (ft_args_isdigit(argv[i]) == 1 || check_max_min(argv[i]))
            return (1);
        i++;
    }
	i = 1;
	while (i < argc)
	{
		j = i + 1;
		while (j < argc)
		{
			if ((ft_atol(argv[i]) - ft_atol(argv[j]) == 0))
				return (1);
			j++;
		}
		i++;
	}
    return (0);
}

int check_argument(int argc, char **argv)
{
    if (argc == 2 && ft_is_space_inside(argv[1]) == 1)
    {
            write(2, "Error\n", 6);
            exit (EXIT_FAILURE);
    }
    else if (argc >= 2)
    {
        if (check_nb(argc, argv) == 1)
        {
            write(2, "Error\n", 6);
            exit (EXIT_FAILURE);
        }
    }
    else
        exit (EXIT_FAILURE);
    return (0);
}
