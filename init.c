/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 18:05:44 by hakalkan          #+#    #+#             */
/*   Updated: 2026/06/09 18:05:44 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_forks(t_data *data)
{
	int	i;
	int	fork_count;

	fork_count = data->num_philos;
	if (data->num_philos == 1)
		fork_count = 1;

	data->forks = malloc(sizeof(pthread_mutex_t) * fork_count);
	if (!data->forks)
		return (1);

	i = 0;
	while (i < fork_count)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
	return (0);
}

int	init_philos(t_data *data)
{
	int	i;

	data->philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!data->philos)
		return (1);

	i = 0;
	while (i < data->num_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals_eaten = 0;
		data->philos[i].last_meal = 0;
		data->philos[i].data = data;

		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[
			(i + 1) % data->num_philos
		];

		pthread_mutex_init(&data->philos[i].last_meal_mutex, NULL);

		i++;
	}
	return (0);
}

void	print_status(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->data->print_mutex);

	if (!get_sim_end(philo->data))
	{
		printf("%ld %d %s\n",
			get_time_ms() - philo->data->start_time,
			philo->id,
			msg);
	}

	pthread_mutex_unlock(&philo->data->print_mutex);
}

int	get_sim_end(t_data *data)
{
	int value;

	pthread_mutex_lock(&data->sim_mutex);
	value = data->simulation_end;
	pthread_mutex_unlock(&data->sim_mutex);
	return (value);
}

void	set_sim_end(t_data *data)
{
	pthread_mutex_lock(&data->sim_mutex);
	data->simulation_end = 1;
	pthread_mutex_unlock(&data->sim_mutex);
}

int	init_all(t_data *data)
{
	pthread_mutex_init(&data->sim_mutex, NULL);
	if (init_forks(data))
		return (1);
	if (init_philos(data))
		return (1);
	if (pthread_mutex_init(&data->print_mutex, NULL))
		return (1);
	return (0);
}

void	cleanup(t_data *data)
{
	int	i;

	if (!data)
		return ;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_destroy(&data->philos[i].last_meal_mutex);
		i++;
	}

	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}

	pthread_mutex_destroy(&data->print_mutex);

	if (data->forks)
		free(data->forks);
	if (data->philos)
		free(data->philos);
}