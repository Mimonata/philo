/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_threads.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 16:49:28 by spitul            #+#    #+#             */
/*   Updated: 2025/01/21 19:53:02 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	wait_all_threads(t_dinner *d)
{
	bool	ready;

	ready = false;
	while (!ready)
	{
		ready = get_bool(&d->mtx_print, &d->all_ready);
		usleep(100);
	}
}

static int	single_philo(t_philo *f)
{
	t_dinner	*din;

	din = f->dinner_data;
	while (!get_bool(&din->mtx_end, &din->end_din) && (timestamp() - get_long(f,
				&din->states[f->index - 1][LAST_EAT]) < get_long(f,
				&din->time_die)))
	{
		pthread_mutex_lock(&din->mtx_forks[f->index - 1]);
		printing(f, TAKES_LEFTFORK);
		usleep(din->time_die * 1000);
		pthread_mutex_unlock(&din->mtx_forks[f->index - 1]);
	}
	return (1);
}

void	*start_routine(void *arg)
{
	t_philo	*f;
	int		right;

	f = (t_philo *)arg;
	right = f->index - 1;
	wait_all_threads(f->dinner_data);
	if (f->index % 2 == 0)
		usleep(300);
	if (f->dinner_data->nb_phil == 1)
	{
		single_philo(f);
		return ((void *)f);
	}
	while (!get_bool(&f->dinner_data->mtx_end, &f->dinner_data->end_din))
	{
		dinner_synchro(f, right);
		sleeping(f);
		thinking(f);
	}
	return ((void *)f);
}

int	start_phil_threads(t_dinner *d, t_philo *f, pthread_t *th)
{
	int	i;

	i = 0;
	d->start_time = timestamp();
	while (i < d->nb_phil)
	{
		init_philo_th(&f[i], d, i);
		if (pthread_create(&th[i], NULL, &start_routine, &f[i]) != 0)
		{
			cleanup_th(d, f, th, i);
			return (print_error("Failure creating thread"));
		}
		i++;
	}
	set_bool(&d->mtx_print, &d->all_ready, true);
	d->mon_ready = true;
	return (1);
}

int	prepare_din_sim(int nb_phil, t_dinner *d)
{
	pthread_t	*th;
	t_philo		*f;

	th = malloc(nb_phil * sizeof(pthread_t));
	if (!th)
		return (cleanup_din(d, "Threads allocation failed"));
	f = malloc(nb_phil * sizeof(t_philo));
	if (!f)
	{
		free(th);
		return (cleanup_din(d, "Philo_t allocation failed"));
	}
	start_phil_threads(d, f, th);
	start_monitor(d);
	cleanup_th(d, f, th, d->nb_phil);
	return (0);
}
