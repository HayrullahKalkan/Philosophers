#include "philo.h"

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;

	printf("Philo %d started\n", philo->id);

	while (1)
		usleep(1000);
	return (NULL);
}
