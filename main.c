/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 15:45:33 by spitul            #+#    #+#             */
/*   Updated: 2024/12/17 07:38:14 by spitul           ###   ########.fr       */
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
	d->states = 0;
	pthread_mutex_init(&d->mutex_states, NULL);
	pthread_mutex_init(&d->mutex_print, NULL);
}

void	parse_input(int argc, char **argv, dinner_t d)
{
	d.nb_phil = ft_atol_phil(argv[1]);
	if (d.nb_phil > 200)
	{
		printf("Number of philosophers cannot be greater than 200\n");
		return ;
	}
	d.time_die = ft_atol_phil(argv[2]);
	d.time_eat = ft_atol_phil(argv[3]);
	d.time_sleep = ft_atol_phil(argv[4]);
	if (d.time_die < 60 || d.time_eat < 60 || d.time_sleep < 60)
	{
		printf("Times must be greater than 60 milliseconds\n");
		return ;
	}
	if (argc == 6)
		d.eating_times = ft_atol_phil(argv[5]);
}

int	main(int argc, char **argv)
{
	dinner_t	d;
	int			i;

	init_dinner(&d);
	i = 0;
	// isdigit or input check or error return from atod
	if (argc == 5 || argc == 6)
	{
		parse_input(argc, argv, d);
	}
	else
		write(1, "wrong input\n", 12);
	// this does not belong here vielleicht doch
	d.mutex_chops = malloc(((d.nb_phil) * sizeof(pthread_mutex_t)));
	d.states = malloc((d.nb_phil) * sizeof(long[2]));
	if (!d.states)
		// error();
	while (i < d.nb_phil)
	{
		pthread_mutex_init(&d.mutex_chops[i], NULL);
		d.states[i][LAST_EAT] = 0;
		d.states[i][MEALS_EATEN] = 0;
		i++;
	}
	d.chops = malloc((d.nb_phil * sizeof(int)));
	memset(d.chops, -1, d.nb_phil * sizeof(int));
	create_threads(d.nb_phil, &d);
	return (0);
}
