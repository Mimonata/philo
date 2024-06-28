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

void	release_forks(philo_t *f, int left, int right)
{
	pthread_mutex_unlock(&f->data->forks[left]);
	pthread_mutex_unlock(&f->data->forks[right]);
}

void	take_forks(philo_t *f, long start, int left, int right)
{
	long	time;
	
	time = timestamp() - start;
	if (f->index % 2) 
	{
		pthread_mutex_lock(&f->data->forks[left]);
		printf("%ld %d has taken left fork\n", time, f->index);
		f->data->chopst[left] = f->index;
		pthread_mutex_lock(&f->data->forks[right]);
		printf("%ld %d has taken right fork\n", time, f->index);
		f->data->chopst[right] = f->index;
	}
	else
	{
		pthread_mutex_lock(&f->data->forks[right]);
		printf("%ld %d has taken right fork\n", time, f->index);
		f->data->chopst[right] = f->index;
		pthread_mutex_lock(&f->data->forks[left]);
		printf("%ld %d has taken left fork\n", time, f->index);
		f->data->chopst[left] = f->index;
	}
	if (f->data->chopst[left] == f->index && f->data->chopst[right] == f->index)
	{
		printf("%ld %d is eating", time, f->index);
		while (timestamp() - time < f->data->time_eat)
			usleep(100);
		f->last_eat = timestamp();
	}
	else
	release_forks(f, left, right);
}

void	*start_routine(void *arg)
{
	long	start;
	philo_t	*f;
	int		left;
	int		right;

	f = (philo_t *)arg;
	if (f->index == 0)
		left = f->data->nb_phil - 1;
	else
		left = f->index - 1;
	right = f->index;
	pthread_mutex_init(&f->data->forks[left], NULL);
	pthread_mutex_init(&f->data->forks[right], NULL);
	start = timestamp();
	while (timestamp() - start < f->data->time_die)
		take_forks(f, start, left, right);
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
