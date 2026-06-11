/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 18:05:50 by hakalkan          #+#    #+#             */
/*   Updated: 2026/06/09 18:05:50 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_usleep(long time)
{
	long start;

	start = get_time_ms();
	while (get_time_ms() - start < time)
		usleep(50);
}

static void	eat(t_philo *philo)
{
	pthread_mutex_t *first;
	pthread_mutex_t *second;
	pthread_mutex_lock(&philo->last_meal_mutex);
	philo->last_meal = get_time_ms();
	pthread_mutex_unlock(&philo->last_meal_mutex);
	if (philo->left_fork < philo->right_fork)
	{
		first = philo->left_fork;
		second = philo->right_fork;
	}
	else
	{
		first = philo->right_fork;
		second = philo->left_fork;
	}

	pthread_mutex_lock(first);
	print_status(philo, "has taken a fork");

	pthread_mutex_lock(second);
	print_status(philo, "has taken a fork");

	pthread_mutex_lock(&philo->last_meal_mutex);
	philo->last_meal = get_time_ms();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->last_meal_mutex);

	print_status(philo, "is eating");
	ft_usleep(philo->data->time_to_eat);

	pthread_mutex_unlock(second);
	pthread_mutex_unlock(first);
}

static void sleeping(t_philo *philo)
{
	print_status(philo, "is sleeping");
	ft_usleep(philo->data->time_to_sleep);
}

static void	thinking(t_philo *philo)
{
	print_status(philo, "is thinking");
}

void	*routine(void *arg)
{
	t_philo *philo = (t_philo *)arg;

	if (philo->data->num_philos == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		ft_usleep(philo->data->time_to_die);
		pthread_mutex_unlock(philo->left_fork);
		return (NULL);
	}

	if (philo->id % 2 == 0)
		usleep(500);

	while (!get_sim_end(philo->data))
	{
		eat(philo);

		if (philo->data->must_eat_count != -1
			&& philo->meals_eaten >= philo->data->must_eat_count)
			break ;

		sleeping(philo);

		thinking(philo);
	}

	return (NULL);
}

void	*monitor(void *arg)
{
	t_data	*data = (t_data *)arg;
	int		i;
	int		all_ate;

	while (!get_sim_end(data))
	{
		i = 0;
		all_ate = 1;

		while (i < data->num_philos)
		{
			pthread_mutex_lock(&data->philos[i].last_meal_mutex);

			if (get_time_ms() - data->philos[i].last_meal > data->time_to_die)
			{
				pthread_mutex_unlock(&data->philos[i].last_meal_mutex);

				pthread_mutex_lock(&data->print_mutex);
				printf("%ld %d died\n",
					get_time_ms() - data->start_time,
					data->philos[i].id);
				pthread_mutex_unlock(&data->print_mutex);

				set_sim_end(data);
				return (NULL);
			}

			if (data->must_eat_count != -1
				&& data->philos[i].meals_eaten < data->must_eat_count)
				all_ate = 0;

			pthread_mutex_unlock(&data->philos[i].last_meal_mutex);
			i++;
		}

		if (all_ate && data->must_eat_count != -1)
		{
			set_sim_end(data);
			return (NULL);
		}

		usleep(100);
	}
	return (NULL);
}
