
#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

typedef struct s_data
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat_count;

	int				simulation_end;

	long			start_time;

	pthread_mutex_t	print_mutex;
	pthread_mutex_t	death_mutex;
	pthread_mutex_t	meal_mutex;

	pthread_mutex_t	*forks;

	struct s_philo	*philos;

}	t_data;


