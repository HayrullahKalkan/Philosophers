/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 18:05:55 by hakalkan          #+#    #+#             */
/*   Updated: 2026/06/09 18:05:55 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

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

void	print_status(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->data->sim_mutex);
	if (philo->data->simulation_end == 0)
	{
		pthread_mutex_lock(&philo->data->print_mutex);
		printf("%ld %d %s\n",
			get_time_ms() - philo->data->start_time,
			philo->id,
			msg);
		pthread_mutex_unlock(&philo->data->print_mutex);
	}
	pthread_mutex_unlock(&philo->data->sim_mutex);
}
