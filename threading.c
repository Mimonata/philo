/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threading.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 16:49:28 by spitul            #+#    #+#             */
/*   Updated: 2024/12/03 06:53:24 by spitul           ###   ########.fr       */
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
			break ;
		usleep(1);
	}
	if (timestamp() - f->last_eat >= f->dinner_data->time_die)
	{
		printf("%ld %d has died", timestamp() - f->dinner_data->start_time,
			f->index);
		// halt_all
	}
}

void	sleeping(long time, philo_t *f)
{
	printf("%ld %d is sleeping\n", time - f->dinner_data->start_time, f->index);
	while (timestamp() - time < f->dinner_data->time_sleep)
		usleep(1);
	thinking(timestamp() - f->dinner_data->start_time, f);
	take_forks(f, f->index);
}

void	release_forks(philo_t *f, int left, int right)
{
	if (f->dinner_data->chops[left] == f->index)
	{
		pthread_mutex_unlock(&f->dinner_data->mutex_chops[left]);
		printf("%ld %d has released left fork\n", timestamp()
			- f->dinner_data->start_time, f->index);
		f->dinner_data->chops[left] = -1;
	}
	if (f->dinner_data->chops[right] == f->index)
	{
		pthread_mutex_unlock(&f->dinner_data->mutex_chops[right]);
		printf("%ld %d has released right fork\n", timestamp()
			- f->dinner_data->start_time, f->index);
		f->dinner_data->chops[right] = -1;
	}
}

void	eating(long time, philo_t *f, int right)
{
	long	current;

	printf("%ld %d is eating\n", time - f->dinner_data->start_time, f->index);
	while (timestamp() - time < f->dinner_data->time_eat)
		usleep(100);
	f->dinner_data->states[f->index][LAST_EAT] = timestamp();
	f->dinner_data->states[f->index][MEALS_EATEN] ++;
	//f->last_eat = timestamp();
	release_forks(f, f->left, right);
	current = timestamp();
	sleeping(current, f);
}

int	take_forks(philo_t *f, int right)
{
	printf("%ld %d is trying to pick up chops \n", timestamp()
		- f->dinner_data->start_time, f->index);
	if (f->index % 2) // das hier überdenken
	{
		if (pthread_mutex_lock(&f->dinner_data->mutex_chops[f->left]) == 0)
		{
			printf("%ld %d has taken left fork\n", timestamp()
				- f->dinner_data->start_time, f->index);
			f->dinner_data->chops[f->left] = f->index;
			if (pthread_mutex_lock(&f->dinner_data->mutex_chops[right]) == 0)
			{
				printf("%ld %d has taken right fork\n", timestamp()
					- f->dinner_data->start_time, f->index);
				f->dinner_data->chops[right] = f->index;
				eating(timestamp(), f, right);
				return (1);
			}
			else
				release_forks(f, f->left, right);
		}
		else
			release_forks(f, f->left, right);
	}
	else
	{
		if (pthread_mutex_lock(&f->dinner_data->mutex_chops[right]) == 0)
		{
			printf("%ld %d has taken right fork\n", timestamp()
				- f->dinner_data->start_time, f->index);
			f->dinner_data->chops[right] = f->index;
			if (pthread_mutex_lock(&f->dinner_data->mutex_chops[f->left]) == 0)
			{
				printf("%ld %d has taken left fork\n", timestamp()
					- f->dinner_data->start_time, f->index);
				f->dinner_data->chops[f->left] = f->index;
				eating(timestamp(), f, right);
				return (1);
			}
			else
				release_forks(f, f->left, right);
		}
		else
			release_forks(f, f->left, right);
	}
	thinking(timestamp() - f->dinner_data->start_time, f);
	return (0);
}

void	*start_monitor(void *arg)
{
	philo_t	*m;

	
}

void	*start_routine(void *arg)
{
	philo_t	*f;
	int		right;

	f = (philo_t *)arg;
	right = f->index;
	if (timestamp() - f->last_eat < f->dinner_data->time_die) // why this while
		take_forks(f, right);
	// else
	return ((void *)f); // whawhawha
}

void	init_philot(philo_t *f, dinner_t *d, int i)
{
	f->last_eat = d->start_time;
	f->meals_nb = 0;
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
	int			i;

	th = malloc(nb_phil * sizeof(pthread_t));
	if (!th)
		error(); //consider here what needs to be deallocated
	f = malloc(nb_phil * sizeof(philo_t)); //dealloc!!
	if (!f)
		error();
	i = 1;
	if (pthread_create(&th[0], NULL, &start_monitor, &f[0]) != 0)
	{
		printf("**Error: can't create thread**");
		return (1);			
	}
	d->start_time = timestamp();
	while (i < nb_phil)
	{
		init_philot(&f[i], d, i);
		if (pthread_create(&th[i], NULL, &start_routine, &f[i]) != 0)
		{
			printf("**Error: can't create thread**");
			return (1);
		}
		usleep(100);
		i++;
	}
	i = 0;
	while (i < nb_phil)
	{
		pthread_join(th[i], NULL); // is the arg needed for anything,
			//maybe last_eat
		free_struct(f[i]); //todo
		free(th[i]);
		i++;
	}
	return (0);
}
