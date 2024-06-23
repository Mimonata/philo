/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threading.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 16:49:28 by spitul            #+#    #+#             */
/*   Updated: 2024/06/23 19:03:23 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000LL));
}	

void	*start_routine(void *arg)
{
	long long	ms;
	philo_t	*fil;
	fil = (philo_t *)arg;

	if (fil->index % 2 == 0)
		fil->data->time_eat += 2;
	else
		fil->data->time_die += 3;  
	printf("%lld this is thread %d \n", timestamp(), fil->index);
	printf("%lld this is the time to die %f \n", timestamp(), fil->data->time_die);
	printf("this is the time to eat %f \n", fil->data->time_eat);
	printf("this is the time to sleep %f \n", fil->data->time_sleep);
	printf("nb_phil from %d is %d\n", fil->index, fil->data->nb_phil);
	return (fil);
}

int	create_threads(int nb_phil, dinner_t *d)
{
	pthread_t	*th;
	philo_t		*f;
	int	i;

	th = malloc(nb_phil * sizeof(pthread_t));
	f = malloc(nb_phil * sizeof(philo_t));
	i = 0;
	while (i < nb_phil)
	{
		f[i].index = i + 1;
		f[i].data = d;
		if (pthread_create(&th[i], NULL, &start_routine, &f[i]) != 0)
		{
			write(1, "can't create thread", 18);
			return (1);
		}
		usleep(1000);
		i ++;	
	}
	return (0);
}
