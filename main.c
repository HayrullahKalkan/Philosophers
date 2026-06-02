#include "philosophers.h"

void *routines(void *arg)
{
    int i = 0;

    while (i<5)
    {
        printf("hello %d\n", i);
        usleep(100000);
        i++;
    }
    return (NULL);
}

static int	ft_isdigit(int c)
{
    if(c >= '0' && c <= '9')
        return (1);
	return (0);
}

long	ft_atol(char *str)
{
	long	res;
	int		sign;
	int		i;

	sign = 1;
	res = 0;
	i = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
		res = res * 10 + (str[i++] - '0');
	return (res * sign);
}

void parser(int ac, char **av, t_data *data)
{
    int i = 1;
    int *data_array;
    
    data_array = malloc((ac - 1) * sizeof(int));
    while(av[i])
    {
        if (!ft_isdigit(av[i][0]))
        {
            printf("Error: Invalid argument\n");
            exit(1);
        }
        data_array[i - 1] = ft_atol(av[i]);
        i++;
    }
}
int main(int ac, char **av)
{
    /*pthread_t thread;

    pthread_create(&thread, NULL, routines, NULL);

    while (1)
    {
        printf("main\n");
        usleep(100000);
    }*/
    t_data data;
    parser(ac, av, &data);

}
