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

static int	take_forks(t_philo *philo,
	pthread_mutex_t **first,
	pthread_mutex_t **second)
{
	if (philo->left_fork < philo->right_fork)
	{
		*first = philo->left_fork;
		*second = philo->right_fork;
	}
	else
	{
		*first = philo->right_fork;
		*second = philo->left_fork;
	}
	pthread_mutex_lock(*first);
	if (get_sim_end(philo->data))
		return (pthread_mutex_unlock(*first), 1);
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(*second);
	if (get_sim_end(philo->data))
	{
		pthread_mutex_unlock(*second);
		pthread_mutex_unlock(*first);
		return (1);
	}
	print_status(philo, "has taken a fork");
	return (0);
}

static void	eat(t_philo *philo)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	if (take_forks(philo, &first, &second))
		return ;
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = get_time_ms();
	pthread_mutex_unlock(&philo->meal_mutex);
	print_status(philo, "is eating");
	ft_usleep(philo->data->time_to_eat, philo);
	pthread_mutex_lock(&philo->state_mutex);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->state_mutex);
	pthread_mutex_unlock(second);
	pthread_mutex_unlock(first);
}

static void	sleeping(t_philo *philo)
{
	print_status(philo, "is sleeping");
	ft_usleep(philo->data->time_to_sleep, philo);
}

static void	thinking(t_philo *philo)
{
	print_status(philo, "is thinking");
	usleep(500);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->data->num_philos == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		ft_usleep(philo->data->time_to_die, philo);
		pthread_mutex_unlock(philo->left_fork);
		return (NULL);
	}
	if (philo->id % 2 == 1)
		usleep(10000);
	while (!get_sim_end(philo->data))
	{
		eat(philo);
		sleeping(philo);
		thinking(philo);
	}
	return (NULL);
}
