/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/13 17:01:15 by hakalkan          #+#    #+#             */
/*   Updated: 2026/06/13 17:01:15 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_death(t_data *data, int i)
{
	long	last_meal;
	long	now;

	pthread_mutex_lock(&data->philos[i].meal_mutex);
	last_meal = data->philos[i].last_meal;
	pthread_mutex_unlock(&data->philos[i].meal_mutex);
	now = get_time_ms();
	if (now - last_meal > data->time_to_die)
	{
		pthread_mutex_lock(&data->sim_mutex);
		if (!data->simulation_end)
		{
			data->simulation_end = 1;
			pthread_mutex_lock(&data->print_mutex);
			printf("%ld %d died\n",
				now - data->start_time,
				data->philos[i].id);
			pthread_mutex_unlock(&data->print_mutex);
		}
		pthread_mutex_unlock(&data->sim_mutex);
		return (1);
	}
	return (0);
}

static int	check_meals(t_data *data)
{
	int	i;
	int	meals;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_lock(&data->philos[i].state_mutex);
		meals = data->philos[i].meals_eaten;
		pthread_mutex_unlock(&data->philos[i].state_mutex);
		if (meals < data->must_eat_count)
			return (0);
		i++;
	}
	return (1);
}

void	*monitor(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (!get_sim_end(data))
	{
		i = 0;
		while (i < data->num_philos)
		{
			if (check_death(data, i))
				return (NULL);
			i++;
		}
		if (data->must_eat_count != -1
			&& check_meals(data))
		{
			set_sim_end(data);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}
