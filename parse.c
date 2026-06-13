/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 18:05:52 by hakalkan          #+#    #+#             */
/*   Updated: 2026/06/09 18:05:52 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

long	ft_atol(const char *str)
{
	long	res;
	int		i;

	res = 0;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (-1);
		if (res > 2147483647 / 10)
			return (-1);
		res = res * 10 + (str[i] - '0');
		if (res > 2147483647)
			return (-1);
		i++;
	}
	return (res);
}

int	parse_args(int ac, char **av, t_data *data)
{
	if (ac != 5 && ac != 6)
		return (1);
	data->num_philos = ft_atol(av[1]);
	data->time_to_die = ft_atol(av[2]);
	data->time_to_eat = ft_atol(av[3]);
	data->time_to_sleep = ft_atol(av[4]);
	data->simulation_end = 0;
	if (data->num_philos <= 0
		|| data->time_to_die <= 0
		|| data->time_to_eat <= 0
		|| data->time_to_sleep <= 0)
		return (1);
	if (ac == 6)
		data->must_eat_count = ft_atol(av[5]);
	else
		data->must_eat_count = -1;
	return (0);
}
