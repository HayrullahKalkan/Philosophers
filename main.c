/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 18:05:47 by hakalkan          #+#    #+#             */
/*   Updated: 2026/06/09 18:05:47 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	start_threads(t_data *data)
{
	int			i;
	pthread_t	monitor_thread;

	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_create(&data->philos[i].thread, NULL,
			routine, &data->philos[i]) != 0)
			return (1);
		i++;
	}
	if (pthread_create(&monitor_thread, NULL, monitor, data) != 0)
		return (1);
	pthread_join(monitor_thread, NULL);
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

	data.start_time = get_time_ms();
	int i;

	i = 0;
	while (i < data.num_philos)
	{
		data.philos[i].last_meal = data.start_time;
		i++;
	}
	start_threads(&data);

	cleanup(&data);
	return (0);
}