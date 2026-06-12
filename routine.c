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
		usleep(500);
}

static void	eat(t_philo *philo)
{
	pthread_mutex_t *first;
	pthread_mutex_t *second;

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

	// update last_meal BEFORE eating window
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = get_time_ms();
	pthread_mutex_unlock(&philo->meal_mutex);

	print_status(philo, "is eating");
	ft_usleep(philo->data->time_to_eat);

	// update meals
	pthread_mutex_lock(&philo->state_mutex);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->state_mutex);

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
	usleep(500);
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

	// stronger stagger (IMPORTANT FIX)
	if (philo->id % 2 == 0)
		usleep(1000);

	while (!get_sim_end(philo->data))
	{
		eat(philo);
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
	long	last_meal;
	int		meals;

	while (!get_sim_end(data))
	{
		i = 0;
		all_ate = (data->must_eat_count != -1);

		while (i < data->num_philos)
		{
			pthread_mutex_lock(&data->philos[i].meal_mutex);
			last_meal = data->philos[i].last_meal;
			pthread_mutex_unlock(&data->philos[i].meal_mutex);

			if (get_time_ms() - last_meal >= data->time_to_die)
			{
				pthread_mutex_lock(&data->sim_mutex);
				if (!data->simulation_end)
				{
					data->simulation_end = 1;
					pthread_mutex_lock(&data->print_mutex);
					printf("%ld %d died\n",
						get_time_ms() - data->start_time,
						data->philos[i].id);
					pthread_mutex_unlock(&data->print_mutex);
				}
				pthread_mutex_unlock(&data->sim_mutex);
				return (NULL);
			}

			if (data->must_eat_count != -1)
			{
				pthread_mutex_lock(&data->philos[i].state_mutex);
				meals = data->philos[i].meals_eaten;
				pthread_mutex_unlock(&data->philos[i].state_mutex);

				if (meals < data->must_eat_count)
					all_ate = 0;
			}

			i++;
		}

		if (all_ate && data->must_eat_count != -1)
		{
			set_sim_end(data);
			return (NULL);
		}

		usleep(500);
	}
	return (NULL);
}
