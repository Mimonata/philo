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

void	take_forks(philo_t *f)
{
	int	left;
	int	right;

	if (f->index == 0)
		left = f->data->nb_phil - 1;
	else
		left = f->index - 1;
	right = f->index;
	pthread_mutex_init(&mutex_fork, NULL);
	if (f->index == 1 || f->index == f->data->nb_phil)
	{

	}
	else 
		if (f->index / 2) //how to keep a fork an amount of time
		{
			pthread_mutex_lock(&mutex_fork);
			f->data->forks[f->index - 1] = 1;
			printf("%lld %d has taken fork", timestamp(), f->index);
			pthread_mutex_unlock(&mutex_fork);
		}
		else
		{
			pthread_mutex_lock(&mutex_fork);
			//f->data->forks[f->index-2] = 1;
			printf("%lld %d has taken fork", timestamp(), f->index);
			pthread_mutex_unlock(&mutex_fork);
		}
	pthread_mutex_lock(&mutex_fork);

	pthread_mutex_unlock(&mutex_fork);
}

void	*start_routine(void *arg)
{
	long long	ms;
	philo_t	*fil;

	fil = (philo_t *)arg;
	take_forks(fil);
	return (arg);
}

void	init_philot(philo_t *f, dinner_t *d, int i)
{
	f->last_eat = 0LL;
	f->data = d;
	f->index = i;
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
