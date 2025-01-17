/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threading.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 16:49:28 by spitul            #+#    #+#             */
/*   Updated: 2025/01/17 20:12:14 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL); // check
	return ((tv.tv_sec * 1000L) + (tv.tv_usec / 1000L));
}

void	thinking(philo_t *f)
{
	if (!get_bool(f->dinner_data->mtx_end, f->dinner_data->end_din))
		printing(f, THINKING);
}

void	sleeping(philo_t *f)
{
	printing(f, SLEEPING);
	usleep(f->dinner_data->time_sleep * 1000);
	if (get_bool(f->dinner_data->mtx_end, f->dinner_data->end_din))
		return ;
}

void	eating(philo_t *f, int right)
{
	printing(f, EATING);
	set_long(f, &f->dinner_data->states[right][LAST_EAT], timestamp());
	set_long(f, &f->dinner_data->states[right][MEALS_EATEN],
		f->dinner_data->states[right][MEALS_EATEN] + 1);
	usleep(f->dinner_data->time_eat * 1000);
	// while (timestamp() - time < f->dinner_data->time_eat
	// 	&& !get_bool(f->dinner_data->mtx_end, f->dinner_data->end_din))
	// 	usleep(1000);
	if (get_bool(f->dinner_data->mtx_end, f->dinner_data->end_din))
		// dunno ob das nötig, wahrscheinlich nicht
		return ;
}

int	grab_forks(philo_t *f, int fork1, int fork2)
{
	dinner_t	*din;

	din = f->dinner_data;
	if (!get_bool(din->mtx_end, din->end_din) && (timestamp()
			- din->states[f->index][LAST_EAT] < din->time_die))
	{
		pthread_mutex_lock(&din->mtx_forks[fork1]);
		printing(f, TAKES_LEFTFORK);
		pthread_mutex_lock(&din->mtx_forks[fork2]);
		printing(f, TAKES_RIGHTFORK);
		eating(f, f->index);
		pthread_mutex_unlock(&din->mtx_forks[fork1]);
		pthread_mutex_unlock(&din->mtx_forks[fork2]);
		return (1);
	}
	return (0);
}

int	dinner_synchro(philo_t *f, int right)
{
	dinner_t	*din;
	int			res;

	din = f->dinner_data;
	res = -1;
	if (get_bool(din->mtx_end, din->end_din))
		return (0);
	if (f->index % 2 == 0 && !get_bool(din->mtx_end, din->end_din))
		// das hier überdenken
		res = grab_forks(f, f->left, right);
	else if (f->index % 2 == 1 && !get_bool(din->mtx_end, din->end_din))
		res = grab_forks(f, right, f->left);
	return (res); // dunno if needed
}

void	*start_routine(void *arg)
{
	philo_t	*f;

	// int		right;
	// right = f->index;
	f = (philo_t *)arg;
	wait_all_threads(f->dinner_data);
	if (f->index % 2 == 0)
		usleep(100);
	while (!get_bool(f->dinner_data->mtx_end, f->dinner_data->end_din))
	{
		dinner_synchro(f, f->index);
		sleeping(f);
		thinking(f);
	}
	return ((void *)f);
}

void	init_philo_th(philo_t *f, dinner_t *d, int i)
{
	f->dinner_data = d;
	f->index = i;
	f->left = (f->index - 1 + d->nb_phil) % d->nb_phil;
	// if (f->index == 0)
	// {
	// 	if (d->nb_phil == 1)
	// 		f->left = -1;
	// 	else
	// 		f->left = d->nb_phil - 1;
	// }
	// else
	// 	f->left = f->index - 1;
	set_long(f, &d->states[i][MEALS_EATEN], 0);
	set_long(f, &d->states[i][LAST_EAT], d->start_time);
}

int	prepare_din_sim(int nb_phil, dinner_t *d)
{
	pthread_t	*th;
	philo_t		*f;

	th = malloc(nb_phil * sizeof(pthread_t));
	if (!th)
		return (cleanup_din(d, "Threads allocation failed"));
	f = malloc(nb_phil * sizeof(philo_t));
	if (!f)
	{
		free(th);
		return (cleanup_din(d, "Philo_t allocation failed"));
	}
	start_phil_threads(d, f, th);
	// usleep(1000);
	start_monitor(d);
	cleanup_th(d, f, th, d->nb_phil);
	return (0);
}
