/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_init_data.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 13:08:44 by spitul            #+#    #+#             */
/*   Updated: 2025/01/10 21:41:30 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_dinner(dinner_t *d)
{
	d->nb_phil = 0;
	d->eating_times = -1;
	d->time_die = 0;
	d->time_eat = 0;
	d->time_sleep = 0;
	d->start_time = 0;
	d->one_dead = 0;
	d->all_ready = false;
	pthread_mutex_init(&d->mtx_states, NULL);
	pthread_mutex_init(&d->mtx_print, NULL);
	d->chops = NULL;
	d->mtx_chops = NULL;
	d->states = NULL;
}

int	parse_input(int argc, char **argv, dinner_t d)
{
	d.nb_phil = ft_atol_phil(argv[1]);
	if (d.nb_phil > 200)
	{
		printf("Number of philosophers cannot be greater than 200\n");
		return (0);
	}
	d.time_die = ft_atol_phil(argv[2]);
	d.time_eat = ft_atol_phil(argv[3]);
	d.time_sleep = ft_atol_phil(argv[4]);
	if (d.time_die < 60 || d.time_eat < 60 || d.time_sleep < 60)
	{
		printf("Durations must be at least 60 milliseconds.\n");
		return (0);
	}
	if (argc == 6)
		d.eating_times = ft_atol_phil(argv[5]);
	return (1);
}

int	allocate_resources(dinner_t *d)
{
	int	i;

	i = 0;
	d->mtx_chops = malloc(((d->nb_phil) * sizeof(pthread_mutex_t)));
	if (!d->mtx_chops)
		return(print_error("Memory allocation failed"));
	d->states = malloc((d->nb_phil) * sizeof(long[2]));
	if (!d->states)
		return (cleanup_din(d, "Memory allocation failed"));
	while (i < d->nb_phil)
	{
		pthread_mutex_init(&d->mtx_chops[i], NULL);
		d->states[i][LAST_EAT] = 0;
		d->states[i][MEALS_EATEN] = 0;
		i++;
	}
	d->chops = malloc((d->nb_phil * sizeof(int)));
	if (!d->chops)
		return (cleanup_din(d, "Memory allocation failed"));
	memset(d->chops, -1, d->nb_phil * sizeof(int));
	return (1);
}

void	set_long(dinner_t *d, long *var, long value)
{
	pthread_mutex_lock(&d->mtx_states);
	*var = value;
	pthread_mutex_unlock(&d->mtx_states);
}
