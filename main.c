#include "philo.h"

int	start_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_create(&data->philos[i].thread, NULL,
			routine, &data->philos[i]) != 0)
			return (1);
		i++;
	}
	i = 0;
	while (i < data->num_philos)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_data	data;

	if (parse_args(ac, av, &data))
		return (printf("Error: args\n"), 1);

	if (init_all(&data))
		return (printf("Error: init\n"), 1);

	start_threads(&data);

	return (0);
}