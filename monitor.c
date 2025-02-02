/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 16:22:04 by spitul            #+#    #+#             */
/*   Updated: 2025/01/21 19:55:19 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	wait_monitor(t_dinner *d)
{
	while (!get_bool(&d->mtx_end, &d->mon_ready))
		;
}

void	check_death(t_dinner *d)
{
	int	i;

	i = 0;
	while (i < d->nb_phil && !get_bool(&d->mtx_end, &d->end_din))
	{
		pthread_mutex_lock(&d->mtx_states[i]);
		if (timestamp() - d->states[i][LAST_EAT] > d->time_die)
		{
			set_bool(&d->mtx_end, &d->end_din, true);
			pthread_mutex_lock(&d->mtx_print);
			printf("%ld %d has died\n", timestamp() - d->start_time, i + 1);
			pthread_mutex_unlock(&d->mtx_print);
		}
		pthread_mutex_unlock(&d->mtx_states[i]);
		if (get_bool(&d->mtx_end, &d->end_din))
			break ;
		i++;
	}
}

int	check_meals(t_dinner *d)
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
		set_bool(&d->mtx_end, &d->end_din, true);
		return (1);
	}
	return (0);
}

void	*create_monitor(void *arg)
{
	t_dinner	*d;

	d = (t_dinner *)arg;
	wait_monitor(d);
	while (!get_bool(&d->mtx_end, &d->end_din))
	{
		check_death(d);
		if (d->eating_times != -2)
			check_meals(d);
		usleep(3000);
	}
	return ((void *)d);
}

int	start_monitor(t_dinner *d)
{
	pthread_t	mh;

	if (pthread_create(&mh, NULL, &create_monitor, d) != 0)
	{
		set_bool(&d->mtx_end, &d->end_din, true);
		return (print_error("**Cannot create monitor**"));
	}
	pthread_join(mh, NULL);
	return (1);
}
