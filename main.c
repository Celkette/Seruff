#include "push_swap.h"

int	main(int argc, char **argv)
{
	t_list	*list_b;
	t_list	*list_a ;

	if (argc == 1)
		return (0);
	check_argument(argc, argv);
	list_a = creat_final_list_a(argc, argv);
	if (argc >= 2)
	{
		list_b = ft_creat_list();
		if (!list_a || !list_b)
		{
			ft_free_list(list_a);
			ft_free_list(list_b);
			return (1);
		}
		algo_choice(argc, list_a, list_b);
	}
	ft_free_list(list_a);
	ft_free_list(list_b);
	return (0);
}
