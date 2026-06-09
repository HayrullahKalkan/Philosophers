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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 17:34:24 by hakalkan          #+#    #+#             */
/*   Updated: 2026/06/09 17:34:24 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "philo.h"

static void	eat(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");

		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");

		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
	}
	print_status(philo, "is eating");
	pthread_mutex_lock(&philo->last_meal_mutex);
	philo->last_meal = get_time_ms();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->last_meal_mutex);
	usleep(philo->data->time_to_eat * 1000);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

static void	sleeping(t_philo *philo)
{
	print_status(philo, "is sleeping");
	usleep(philo->data->time_to_sleep * 1000);
}

static void	thinking(t_philo *philo)
{
	print_status(philo, "is thinking");
}

int	check_death(t_philo *philo)
{
	long	current_time;

	pthread_mutex_lock(&philo->last_meal_mutex);
	current_time = get_time_ms();
	if (current_time - philo->last_meal > philo->data->time_to_die)
	{
		pthread_mutex_unlock(&philo->last_meal_mutex);
		print_status(philo, "died");
		pthread_mutex_lock(&philo->data->print_mutex);
		philo->data->simulation_end = 1;
		pthread_mutex_unlock(&philo->data->print_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->last_meal_mutex);
	return (0);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;

	pthread_mutex_lock(&philo->last_meal_mutex);
	philo->last_meal = get_time_ms();
	pthread_mutex_unlock(&philo->last_meal_mutex);

	while (!philo->data->simulation_end)
	{
		eat(philo);
		if (check_death(philo))
			break ;
		if (philo->data->must_eat_count != -1
			&& philo->meals_eaten >= philo->data->must_eat_count)
			break ;
		sleeping(philo);
		if (check_death(philo))
			break ;
		thinking(philo);
	}
	return (NULL);
}