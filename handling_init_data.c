/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_init_data.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 13:08:44 by spitul            #+#    #+#             */
/*   Updated: 2025/01/21 19:42:38 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000L) + (tv.tv_usec / 1000L));
}

void	init_dinner(t_dinner *d)
{
	d->nb_phil = 0;
	d->eating_times = -2;
	d->time_die = 0;
	d->time_eat = 0;
	d->time_sleep = 0;
	d->start_time = 0;
	d->one_dead = 0;
	d->all_ready = false;
	d->end_din = false;
	d->mon_ready = false;
	d->chops = NULL;
	d->mtx_forks = NULL;
	d->mtx_states = NULL;
	d->states = NULL;
	pthread_mutex_init(&d->mtx_print, NULL);
	pthread_mutex_init(&d->mtx_end, NULL);
}

int	parse_input(int argc, char **argv, t_dinner *d)
{
	d->nb_phil = ft_atol_phil(argv[1]);
	if (d->nb_phil > 200)
	{
		printf("Number of philosophers cannot be greater than 200\n");
		return (0);
	}
	d->time_die = ft_atol_phil(argv[2]);
	d->time_eat = ft_atol_phil(argv[3]);
	d->time_sleep = ft_atol_phil(argv[4]);
	if (argc == 6)
		d->eating_times = ft_atol_phil(argv[5]);
	if (d->time_die == -1 || d->time_eat == -1 || d->time_sleep == -1
		|| d->nb_phil == -1 || d->eating_times == -1)
		return (0);
	if (d->time_die < 60 || d->time_eat < 60 || d->time_sleep < 60)
	{
		printf("\x1b[35mDurations must be at least 60 milliseconds.\x1b[0m\n");
		return (0);
	}
	if (d->nb_phil == 0 || d->eating_times == 0)
		return (0);
	return (1);
}

int	allocate_resources(t_dinner *d)
{
	int	i;

	i = 0;
	d->mtx_forks = malloc((d->nb_phil) * sizeof(pthread_mutex_t));
	if (!d->mtx_forks)
		return (print_error("Memory allocation failed"));
	d->states = malloc((d->nb_phil) * sizeof(long[2]));
	if (!d->states)
		return (cleanup_din(d, "Memory allocation failed"));
	d->mtx_states = malloc((d->nb_phil) * sizeof(pthread_mutex_t));
	if (!d->mtx_states)
		return (cleanup_din(d, "Memory allocation failed"));
	while (i < d->nb_phil)
	{
		pthread_mutex_init(&d->mtx_forks[i], NULL);
		pthread_mutex_init(&d->mtx_states[i], NULL);
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

void	init_philo_th(t_philo *f, t_dinner *d, int i)
{
	f->dinner_data = d;
	f->index = i + 1;
	f->left = f->index % d->nb_phil;
	set_long(f, &d->states[i][MEALS_EATEN], 0);
	set_long(f, &d->states[i][LAST_EAT], d->start_time);
}
