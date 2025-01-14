/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 16:22:04 by spitul            #+#    #+#             */
/*   Updated: 2025/01/14 18:43:33 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	wait_all_threads(dinner_t *d)
{
	while (!d->all_ready)
		;
}

void	check_death(dinner_t *d)
{
	int	i;

	i = 0;
	while (i < d->nb_phil && !get_bool(d->mtx_end, d->end_din))
	{
		pthread_mutex_lock(&d->mtx_states[i]);
		if (timestamp() - d->states[i][LAST_EAT] > d->time_die)
		{
			set_bool(d->mtx_end, &d->end_din, true);
			pthread_mutex_lock(&d->mtx_print);
			printf("%ld %d has died\n", timestamp() - d->start_time, i);
			pthread_mutex_unlock(&d->mtx_print);
			break ;
		}
		pthread_mutex_unlock(&d->mtx_states[i]);
		i++;
	}
}

int	check_meals(dinner_t *d)
{
	int	i;
	int	fin;

	i = 0;
	fin = 0;
	while (i < d->nb_phil && fin < d->nb_phil)
	{
		pthread_mutex_lock(&d->mtx_states[i]);
		if (d->states[i][MEALS_EATEN] >= d->eating_times)
			fin++;
		pthread_mutex_unlock(&d->mtx_states[i]);
		i++;
	}
	if (fin == d->nb_phil)
	{
		set_bool(d->mtx_end, &d->end_din, true);
		return (1);
	}
	return (0);
}

void	*create_monitor(void *arg)
{
	dinner_t	*d;

	d = (dinner_t *)arg;
	while (!get_bool(d->mtx_end, d->end_din))
	{
		check_death(d);
		if (d->eating_times != -1)
			check_meals(d);
		usleep(5000);
	}
	return ((void *)d);
}

int	start_monitor(dinner_t *d)
{
	pthread_t	mh;
	int			t;

	if (pthread_create(&mh, NULL, &create_monitor, d) != 0)
	{
		set_bool(d->mtx_end, &d->end_din, true);
		return (print_error("**Cannot create monitor**"));
	}
	// pthread_join(m, NULL);
	return (1);
}
