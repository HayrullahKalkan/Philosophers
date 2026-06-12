/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 03:12:56 by hakalkan          #+#    #+#             */
/*   Updated: 2026/06/12 03:12:56 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_philo	t_philo;

typedef struct s_data
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat_count;

	int				simulation_end;

	long			start_time;
	
	pthread_mutex_t	finish_mutex;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t sim_mutex;
	t_philo			*philos;

}	t_data;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long			last_meal;

	pthread_t		thread;

	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	state_mutex;   // ✔ meals_eaten için
	pthread_mutex_t	meal_mutex;    // ✔ last_meal için
	//pthread_mutex_t	last_meal_mutex;
	t_data			*data;

}	t_philo;

/* parsing */
int		parse_args(int ac, char **av, t_data *data);
long	ft_atol(const char *str);
int		ft_isdigit(int c);

/* init */
int		init_all(t_data *data);
void	free_func(t_data *data);

/* routine */
void	*routine(void *arg);
void	*monitor(void *arg);

/* utils */
long	get_time_ms(void);
void	print_status(t_philo *philo, char *msg);
int		get_sim_end(t_data *data);
void	set_sim_end(t_data *data);

#endif