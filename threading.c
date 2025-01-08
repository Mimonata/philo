/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threading.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 16:49:28 by spitul            #+#    #+#             */
/*   Updated: 2025/01/08 20:15:14 by spitul           ###   ########.fr       */
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
	printing(f, THINKING, timestamp());
	take_forks(f, f->index);
}

void	sleeping(long time, philo_t *f)
{
	printing(f, SLEEPING, timestamp());
	usleep(f->dinner_data->time_sleep);
	if (f->dinner_data->one_dead == 1)
		return ;
}

void	release_forks(philo_t *f, int left, int right)
{
	if (f->dinner_data->chops[left] == f->index)
	{
		pthread_mutex_unlock(&f->dinner_data->mtx_chops[left]);
		printf("%ld %d has released left fork\n", timestamp()
			- f->dinner_data->start_time, f->index);
		f->dinner_data->chops[left] = 0;
	}
	if (f->dinner_data->chops[right] == f->index)
	{
		pthread_mutex_unlock(&f->dinner_data->mtx_chops[right]);
		printf("%ld %d has released right fork\n", timestamp()
			- f->dinner_data->start_time, f->index);
		f->dinner_data->chops[right] = 0;
	}
}

void	eating(long time, philo_t *f, int right)
{
	long	current;

	pthread_mutex_lock(&f->dinner_data->mtx_states);
	f->dinner_data->states[f->index][LAST_EAT] = timestamp();
	f->dinner_data->states[f->index][MEALS_EATEN]++;
	pthread_mutex_unlock(&f->dinner_data->mtx_states);
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
	if (din->one_dead == 0)
	{
		pthread_mutex_lock(&din->mtx_chops[fork1]);
		printf("%ld %d has taken left fork\n", timestamp() - din->start_time,
			f->index);
		din->chops[fork1] = f->index;
		pthread_mutex_lock(&din->mtx_chops[fork2]);
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
	// int		right;

	// right = f->index;
	f = (philo_t *)arg;
	wait_all_threads(f->dinner_data);
	while (f->dinner_data->one_dead == 0)
	{
		take_forks(f, f->index);
		eating(timestamp(), f, f->index);
		sleeping(timestamp(), f);
		thinking(timestamp(), f);
	}
	// if (f->index % 2 == 0)
	// 	usleep(1500);
	// if (timestamp() - f->last_eat < f->dinner_data->time_die)
	// 	take_forks(f, right);
	return ((void *)f);
}

void	init_philo_th(philo_t *f, dinner_t *d, int i)
{
	//f->last_eat = d->start_time; where to move this 
	f->dinner_data = d;
	// f->dinner_data->states[f->index][MEALS_EATEN] = 0;
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
	pthread_mutex_lock(&f->dinner_data->mtx_states);
	f->dinner_data->states[f->index][LAST_EAT] = f->last_eat; // really??
	pthread_mutex_unlock(&f->dinner_data->mtx_states);
}

int	prepare_din_sim(int nb_phil, dinner_t *d)
{
	pthread_t	*th;
	philo_t		*f;
	int			i;

	th = malloc(nb_phil * sizeof(pthread_t));
	if (!th)
		return (cleanup_din(d->mtx_chops, d->nb_phil, d->states, d->chops, "Threads allocation failed"));
	f = malloc(nb_phil * sizeof(philo_t));
	if (!f)
	{
		free (th);
		return (cleanup_din(d, "Phil_t allocation failed"));
	}
	create_phil_threads(d, f);
	
	d->start_time = timestamp();
	d->all_ready = true;
	cleanup_th(d, f, th, d-nb_phil - 1);
	return (0);
}
