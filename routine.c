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

void	ft_usleep(long time, t_philo *philo)
{
	long	start;

	start = get_time_ms();
	while (get_time_ms() - start < time)
	{
		if (get_sim_end(philo->data))
			return ;
		usleep(50); // 100 değil 50
	}
}

/*static int can_eat(t_philo *philo)
{
	int ok;

	pthread_mutex_lock(&philo->state_mutex);
	ok = (philo->data->must_eat_count == -1
		|| philo->meals_eaten < philo->data->must_eat_count);
	pthread_mutex_unlock(&philo->state_mutex);

	return ok;
}
*/
static void eat(t_philo *philo)
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
    if (get_sim_end(philo->data))
    {
        pthread_mutex_unlock(first);
        return;
    }
    print_status(philo, "has taken a fork");

    pthread_mutex_lock(second);
    if (get_sim_end(philo->data))
    {
        pthread_mutex_unlock(second);
        pthread_mutex_unlock(first);
        return;
    }
    print_status(philo, "has taken a fork");

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

static void sleeping(t_philo *philo)
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
	t_philo *philo = (t_philo *)arg;

	if (philo->data->num_philos == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		ft_usleep(philo->data->time_to_die,philo);
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

void	*monitor(void *arg)
{
	t_data	*data;
	int		i;
	long	last_meal;
	int		meals;
	int		finished;
	long	now;
	data = (t_data *)arg;

	while (!get_sim_end(data))
	{
		i = 0;
		finished = 1;

		while (i < data->num_philos)
		{
			// 🔥 1. DEATH CHECK (SAFE READ)
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
				return (NULL);
			}

			// 🔥 2. MEALS CHECK (must_eat)
			if (data->must_eat_count != -1)
			{
				pthread_mutex_lock(&data->philos[i].state_mutex);
				meals = data->philos[i].meals_eaten;
				pthread_mutex_unlock(&data->philos[i].state_mutex);

				if (meals < data->must_eat_count)
					finished = 0;
			}

			i++;
		}

		// 🔥 3. ALL DONE CHECK
		if (data->must_eat_count != -1 && finished)
		{
			pthread_mutex_lock(&data->sim_mutex);
			data->simulation_end = 1;
			pthread_mutex_unlock(&data->sim_mutex);
			return (NULL);
		}

		usleep(1000);
	}
	return (NULL);
}