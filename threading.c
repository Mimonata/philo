/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threading.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 16:49:28 by spitul            #+#    #+#             */
/*   Updated: 2024/06/24 19:12:37 by spitul           ###   ########.fr       */
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
	pthread_mutex_t	mutex;
	
	fil = (philo_t *)arg;
	pthread_mutex_init(&mutex, NULL);
	return (arg);
}

void	init_philot(philo_t *f, dinner_t *d, int i)
{
	f->last_eat = 0LL;
	f->data = d;
	f->index = i + 1;
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
		init_philot(&f[i], d, i);
		
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
