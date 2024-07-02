/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threading.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 16:49:28 by spitul            #+#    #+#             */
/*   Updated: 2024/07/02 19:47:01 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000L) + (tv.tv_usec / 1000L));
}	

void	thinking(long time, philo_t *f)
{
	printf("%ld %d is thinking\n", time, f->index);
}

void	sleeping(long time, philo_t *f)
{
	printf("%ld %d is sleeping\n", time, f->index);
	while (timestamp() - time < f->data->time_sleep)
		usleep(1);
	take_forks(f, f->last_eat, f->index);
}

void	release_forks(philo_t *f, int left, int right)
{
	pthread_mutex_unlock(&f->data->forks[left]);
	pthread_mutex_unlock(&f->data->forks[right]);
}

void	eating(long time, philo_t *f, int right)
{
	long	current;
	
	printf("%ld %d is eating\n", time, f->index);
	while (timestamp() - time < f->data->time_eat)
		usleep(1);
	f->last_eat = timestamp();
	release_forks(f, f->left, right);
	current = timestamp();
	sleeping(current, f);
}
void	take_forks(philo_t *f, long start, int right)
{
	long	time;
	
	right = f->index;
	time = timestamp() - start;
	if (f->index % 2) 
	{
		pthread_mutex_lock(&f->data->forks[f->left]);
		printf("%ld %d has taken left fork\n", time, f->index);
		f->data->chopst[f->left] = f->index;
		pthread_mutex_lock(&f->data->forks[right]);
		printf("%ld %d has taken right fork\n", time, f->index);
		f->data->chopst[right] = f->index;
	}
	else
	{
		pthread_mutex_lock(&f->data->forks[right]);
		printf("%ld %d has taken right fork\n", time, f->index);
		f->data->chopst[right] = f->index;
		pthread_mutex_lock(&f->data->forks[f->left]);
		printf("%ld %d has taken left fork\n", time, f->index);
		f->data->chopst[f->left] = f->index;
	}
	if (f->data->chopst[f->left] == f->index && f->data->chopst[right] == f->index)
		eating(timestamp() - start, f, right);
	else
		thinking(timestamp() - start, f);
}

void	*start_routine(void *arg)
{
	long	start;
	philo_t	*f;
	int		left;
	int		right;

	f = (philo_t *)arg;
	
	right = f->index;
	pthread_mutex_init(&f->data->forks[left], NULL);
	pthread_mutex_init(&f->data->forks[right], NULL);
	start = timestamp();
	while (timestamp() - start < f->data->time_die)
		take_forks(f, start, right);
	return (arg);
}

void	init_philot(philo_t *f, dinner_t *d, int i)
{
	f->last_eat = 0LL;
	f->data = d;
	f->index = i;
	if (f->index == 0)
		f->left = f->data->nb_phil - 1;
	else
		f->left = f->index - 1;
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
