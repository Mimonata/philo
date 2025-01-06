/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threading.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 16:49:28 by spitul            #+#    #+#             */
/*   Updated: 2025/01/06 07:45:48 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL); // check
	return ((tv.tv_sec * 1000L) + (tv.tv_usec / 1000L));
}

void	thinking(long time, philo_t *f)
{
	// long	time_var;
	printf("%ld %d is thinking\n", time, f->index);
	take_forks(f, f->index);
}

void	sleeping(long time, philo_t *f)
{
	printf("%ld %d is sleeping\n", time - f->dinner_data->start_time, f->index);
	while (timestamp() - time < f->dinner_data->time_sleep
		&& f->dinner_data->one_dead == 0)
		usleep(1);
	if (f->dinner_data->one_dead == 1)
		return ;
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
		f->dinner_data->chops[left] = 0;
	}
	if (f->dinner_data->chops[right] == f->index)
	{
		pthread_mutex_unlock(&f->dinner_data->mutex_chops[right]);
		printf("%ld %d has released right fork\n", timestamp()
			- f->dinner_data->start_time, f->index);
		f->dinner_data->chops[right] = 0;
	}
}

void	eating(long time, philo_t *f, int right)
{
	long	current;

	pthread_mutex_lock(&f->dinner_data->mutex_states);
	f->dinner_data->states[f->index][LAST_EAT] = timestamp();
	f->dinner_data->states[f->index][MEALS_EATEN]++;
	pthread_mutex_unlock(&f->dinner_data->mutex_states);
	printf("\x1b[38;2;120;0;255m%ld %d is eating\x1b[0m\n", time
		- f->dinner_data->start_time, f->index);
	while (timestamp() - time < f->dinner_data->time_eat
		&& f->dinner_data->one_dead == 0)
		usleep(100);
	if (f->dinner_data->one_dead == 1)
		return ;
	// f->last_eat = timestamp();
	release_forks(f, f->left, right);
	current = timestamp();
	sleeping(current, f);
}

int	grab_forks(philo_t *f, int fork1, int fork2)
{
	dinner_t	*din;

	din = f->dinner_data;
	if (din->one_dead == 1)
		return (0);
	if (din->one_dead == 0)
	{
		pthread_mutex_lock(&din->mutex_chops[fork1]);
		printf("%ld %d has taken left fork\n", timestamp() - din->start_time,
			f->index);
		din->chops[fork1] = f->index;
		pthread_mutex_lock(&din->mutex_chops[fork2]);
		printf("%ld %d has taken right fork\n", timestamp() - din->start_time,
			f->index);
		din->chops[fork2] = f->index;
		eating(timestamp(), f, f->index);
		return (1);
	}
	else
		return (0);
}

int	take_forks(philo_t *f, int right)
{
	dinner_t	*din;
	int			res;

	// printf("\x1b[38;2;189;252;201m%ld%d is trying to pick up chops \x1b[0m\n",
	// timestamp() - f->dinner_data->start_time, f->index);
	din = f->dinner_data;
	res = -1;
	if (din->one_dead == 1)
		return (0);
	if (f->index % 2 == 0 && din->one_dead == 0) // das hier überdenken
	{
	
		res = grab_forks(f, f->left, right);
	}
	else
	{
		res = grab_forks(f, right, f->left);
	}
	if (res == 0)
		thinking(timestamp() - din->start_time, f);
	return (0);
}

void	*start_routine(void *arg)
{
	philo_t	*f;
	int		right;

	f = (philo_t *)arg;
	while (f->dinner_data->one_dead == 0)
	{
		take_forks(f, f->index);
		eating();
		sleeping(timestamp, f);
		thinking();
	}
	// if (f->index % 2 == 0)
	// 	usleep(1500);
	right = f->index;
	if (timestamp() - f->last_eat < f->dinner_data->time_die)
		take_forks(f, right);
	// else
	return ((void *)f);
}

void	init_philot(philo_t *f, dinner_t *d, int i)
{
	f->last_eat = d->start_time;
	f->dinner_data = d;
	// f->dinner_data->states[f->index][MEALS_EATEN] = 0;
	f->index = i;
	if (f->index == 1)
	{
		if (d->nb_phil == 1)
			f->left = -1;
		else
			f->left = f->dinner_data->nb_phil - 1;
	}
	else
		f->left = f->index - 1;
	pthread_mutex_lock(&f->dinner_data->mutex_states);
	f->dinner_data->states[f->index][LAST_EAT] = f->last_eat;
	pthread_mutex_unlock(&f->dinner_data->mutex_states);
}

int	create_threads(int nb_phil, dinner_t *d)
{
	pthread_t	*th;
	philo_t		*f;
	int			i;

	th = malloc(nb_phil * sizeof(pthread_t));
	// if (!th)
	// error();
	// consider here what needs to be deallocated
	f = malloc(nb_phil * sizeof(philo_t)); // dealloc!!
	// if (!f)
	// 	error();
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
		usleep(100);
		i++;
	}
	i = 0;
	// pthread_detach(th[0]);
	while (i < nb_phil)
	{
		pthread_join(th[i], NULL);
		// free_struct(f[i]);         // todo
		i++;
	}
	free(th);
	return (0);
}
