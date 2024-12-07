/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threading.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 16:49:28 by spitul            #+#    #+#             */
/*   Updated: 2024/12/07 19:15:29 by spitul           ###   ########.fr       */
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
	// while (timestamp() - f->last_eat < f->dinner_data->time_die)
	// {
	// 	if (take_forks(f, f->index) == 1)
	// 		break ;
	// 	usleep(1);
	// }
	// if (timestamp() - f->last_eat >= f->dinner_data->time_die)
	// 	f->dinner_data->one_dead = 1;
	// {
	// 	printf("%ld %d has died", timestamp() - f->dinner_data->start_time,
	// 		f->index);
	// 	// halt_all
	// }
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

int	take_forks(philo_t *f, int right)
{
	dinner_t	*din;

	// printf("\x1b[38;2;189;252;201m%ld%d is trying to pick up chops \x1b[0m\n",
	// timestamp() - f->dinner_data->start_time, f->index);
	din = f->dinner_data;
	if (din->one_dead == 1)
		return (0);
	if (f->index % 2 == 0 && din->one_dead == 0) // das hier überdenken
	{
		pthread_mutex_lock(&din->mutex_chops[f->left]);
		printf("%ld %d has taken left fork\n", timestamp()
				- din->start_time, f->index);
		din->chops[f->left] = f->index;
		pthread_mutex_lock(&din->mutex_chops[right]);
		printf("%ld %d has taken right fork\n", timestamp()
					- din->start_time, f->index);
		din->chops[right] = f->index;
		eating(timestamp(), f, right);
		return (1);
		// if (pthread_mutex_lock(&din->mutex_chops[f->left]) == 0)
		// { // am i still alive and drop the one
		// 	printf("%ld %d has taken left fork\n", timestamp()
		// 		- din->start_time, f->index);
		// 	din->chops[f->left] = f->index;
		// 	if (pthread_mutex_lock(&din->mutex_chops[right]) == 0)
		// 	{ // am i still alive and drop forks
		// 		printf("%ld %d has taken right fork\n", timestamp()
		// 			- din->start_time, f->index);
		// 		din->chops[right] = f->index;
		// 		eating(timestamp(), f, right);
		// 		return (1);
		// 	}
		// }
		else
			release_forks(f, f->left, right);
	}
	else
	{
		if (pthread_mutex_lock(&din->mutex_chops[right]) == 0
			&& din->one_dead == 0)
		{
			printf("%ld %d has taken right fork\n", timestamp()
				- din->start_time, f->index);
			din->chops[right] = f->index;
			if (pthread_mutex_lock(&din->mutex_chops[f->left]) == 0)
			{
				printf("%ld %d has taken left fork\n", timestamp()
					- din->start_time, f->index);
				din->chops[f->left] = f->index;
				eating(timestamp(), f, right);
				return (1);
			}
		}
		else
			release_forks(f, f->left, right);
	}
	thinking(timestamp() - din->start_time, f);
	return (0);
}

void	*start_monitor(void *arg)
{
	philo_t	*m;
	int		i;

	m = (philo_t *)arg;
	i = 1;
	// usleep(1000);
	while (m->dinner_data->one_dead == 0)
	{
		check_death(m);
		check_meals(m);//rethink
	}
	return ((void *)m);
}

void	*start_routine(void *arg)
{
	philo_t	*f;
	int		right;

	f = (philo_t *)arg;
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
	//f->dinner_data->states[f->index][MEALS_EATEN] = 0;
	f->index = i;
	if (f->index == 1)
	{
		if (d->nb_phil == 1)
			f->left = -1; 
		else
			f->left = f->dinner_data->nb_phil;
	}
	else
		f->left = f->index - 1;
	f->dinner_data->states[f->index][LAST_EAT] = f->last_eat;
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
	i = 1;
	d->start_time = timestamp();
	while (i <= nb_phil)
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
	init_philot(&f[0], d, 0);
	if (pthread_create(&th[0], NULL, &start_monitor, &f[0]) != 0)
	{
		printf("**Error: can't create thread**");
		return (1);
	}
	i = 1;
	pthread_detach(th[0]);
	while (i < nb_phil)
	{
		pthread_join(th[i], NULL); // is the arg needed for anything,
									// maybe last_eat
		// free_struct(f[i]);         // todo
		i++;
	}
	free(th);
	return (0);
}
