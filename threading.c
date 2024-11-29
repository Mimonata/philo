/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threading.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 16:49:28 by spitul            #+#    #+#             */
/*   Updated: 2024/11/28 06:58:57 by spitul           ###   ########.fr       */
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
	while (timestamp() - f->last_eat < f->dinner_data->time_die)
	{
		if (take_forks(f, f->index) == 1)
			break;
		usleep(1);
	}
	if (timestamp() - f->last_eat >= f->dinner_data->time_die)
	{
		printf("%ld %d has died", timestamp() - f->dinner_data->start_time, f->index);
		//halt_all
	}
}

void	sleeping(long time, philo_t *f)
{
	printf("%ld %d is sleeping\n", time - f->dinner_data->start_time, f->index);
	while (timestamp() - time < f->dinner_data->time_sleep)
		usleep(1);
	take_forks(f, f->index);
}

void	release_forks(philo_t *f, int left, int right)
{
	if (f->dinner_data->chops[left] == f->index)
	{
		pthread_mutex_unlock(&f->dinner_data->mutex_chops[left]);
		printf("%ld %d has released left fork\n", timestamp() - f->dinner_data->start_time, f->index);
		f->dinner_data->chops[left] = -1;
	}
	if (f->dinner_data->chops[right] == f->index)
	{
		pthread_mutex_unlock(&f->dinner_data->mutex_chops[right]);
		printf("%ld %d has released right fork\n", timestamp() - f->dinner_data->start_time, f->index);
		f->dinner_data->chops[right] = -1;
	}
}

void	eating(long time, philo_t *f, int right)
{
	long	current;
	
	printf("%ld %d is eating\n", time - f->dinner_data->start_time, f->index);
	while (timestamp() - time < f->dinner_data->time_eat)
		usleep(1);
	f->last_eat = timestamp();
	release_forks(f, f->left, right);
	current = timestamp();
	sleeping(current, f);
}

int	take_forks(philo_t *f, int right)
{
	printf("%ld %d is trying to pick up chops \n", timestamp() - f->dinner_data->start_time, f->index);
	if (f->index % 2) // das hier überdenken
	{
		if (pthread_mutex_lock(&f->dinner_data->mutex_chops[f->left]) == 0)
		{
			printf("%ld %d has taken left fork\n", timestamp() - f->dinner_data->start_time, f->index);
			//f->dinner_data->chops[f->left] = f->index;
			if (pthread_mutex_lock(&f->dinner_data->mutex_chops[right]) == 0)
			{
				printf("%ld %d has taken right fork\n", timestamp() - f->dinner_data->start_time, f->index);
				eating(timestamp(), f, right);
				return (1);
			}
			else
				release_forks(f, f->left, right);
			//f->dinner_data->chops[right] = f->index;
		}
		else
			release_forks(f, f->left, right);
	}
	else
	{
		if (pthread_mutex_lock(&f->dinner_data->mutex_chops[right]) == 0)
		{
			printf("%ld %d has taken right fork\n", timestamp() - f->dinner_data->start_time, f->index);
			//f->dinner_data->chops[right] = f->index;
			if (pthread_mutex_lock(&f->dinner_data->mutex_chops[f->left]) == 0)
			{
				printf("%ld %d has taken left fork\n", timestamp() - f->dinner_data->start_time, f->index);
				eating(timestamp(), f, right);
				return (1);
			}
			else
				release_forks(f, f->left, right);
			//f->dinner_data->chops[f->left] = f->index;
		}
		else
			release_forks(f, f->left, right);
	}
	/*if (f->dinner_data->chops[f->left] == f->index && f->dinner_data->chops[right] == f->index)
	{
		eating(timestamp(), f, right);
		return (1);
	}
	else
	{
		release_forks(f, f->left, f->index);
		thinking(timestamp() - f->dinner_data->start_time, f);
	}*/
	return (0);
}

void	*start_routine(void *arg)
{
	philo_t	*f;
	int		right;

	f = (philo_t *)arg;
	right = f->index;
	while (timestamp() - f->dinner_data->start_time < f->dinner_data->time_die) //why this while
		take_forks(f, right);
	return ((void *)f);
}

void	init_philot(philo_t *f, dinner_t *d, int i)
{
	f->last_eat = -1;
	f->dinner_data = d;
	f->index = i;
	if (f->index == 0)
	{
		if (d->nb_phil == 1)
			f->left = -1;
		else
			f->left = f->dinner_data->nb_phil - 1;
	}
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
	d->start_time = timestamp();
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
	{
		pthread_join(th[i], NULL); //is the arg needed for anything, maybe last_eat
		i ++;
	}
	return (0);
}
