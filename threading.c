/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threading.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 16:49:28 by spitul            #+#    #+#             */
/*   Updated: 2024/06/26 19:32:57 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000L) + (tv.tv_usec / 1000L));
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
	pthread_mutex_init(&f->data->forks[left], NULL);
	pthread_mutex_init(&f->data->forks[right], NULL);
	if (f->index % 2) //how to keep a fork an amount of time
	{
		pthread_mutex_lock(&f->data->forks[left]);
		printf("%ld %d has taken left fork\n", timestamp(), f->index);
		pthread_mutex_unlock(&f->data->forks[left]);
		pthread_mutex_lock(&f->data->forks[right]);
		printf("%ld %d has taken right fork\n", timestamp(), f->index);
		pthread_mutex_unlock(&f->data->forks[right]);
	}
	else
	{
		pthread_mutex_lock(&f->data->forks[right]);
		printf("%ld %d has taken right fork\n", timestamp(), f->index);
		pthread_mutex_unlock(&f->data->forks[right]);
		pthread_mutex_lock(&f->data->forks[left]);
		printf("%ld %d has taken left fork\n", timestamp(), f->index);
		pthread_mutex_unlock(&f->data->forks[left]);
	}
}

void	*start_routine(void *arg)
{
	long	start;
	philo_t	*f;

	f = (philo_t *)arg;
	start = timestamp();
	while (timestamp() - start < f->data->time_die)
	take_forks(f);
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
			printf("**Error: can't create thread**");
			return (1);
		}
		usleep(1000);
		i ++;	
	}
	i = 0;
	while (i < nb_phil)
		pthread_join(th[i], NULL);
	return (0);
}
