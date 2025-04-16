#include "push_swap.h"

int	ft_args_isdigit(char *str)
{
    int i;
    int in_digit;
    
    i = 0;
    in_digit = 0;
    while (str[i] != '\0')
    {
        if (str[i] == '-' || str[i] == '+')
        {
            if (in_digit == 1 || str[i + 1] == '\0' || !ft_isdigit(str[i + 1]))
                return (1);
            in_digit = 1;
        }
        else if (ft_isdigit(str[i]))
            in_digit = 1;
        else
            return (1);
        i++ ;
    }
    return (0);
}

int ft_is_space_inside(const char *str)
{
    int i = 0;
    int digit_seen = 0;

    while (str && str[i] != '\0' && ft_isspace(str[i]))
        i++;
    while (str && str[i] != '\0')
    {
        if (ft_isdigit(str[i]))
            digit_seen = 1;
        else if (ft_isspace(str[i]) && digit_seen)
        {
            int j = i + 1;
            while (str[j] != '\0' && ft_isspace(str[j]))
                j++;
            if (ft_isdigit(str[j]))
                return (1);
        }
        i++;
    }
    return (0);
}
