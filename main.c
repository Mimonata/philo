/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 15:45:33 by spitul            #+#    #+#             */
/*   Updated: 2024/11/29 06:19:36 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_dinner(dinner_t *d)
{
	d->nb_phil = 0;
	d->eating_times = 0;
	d->time_die = 0;
	d->time_eat = 0;
	d->time_sleep = 0;
	d->start_time = 0;
	d->one_dead = 0;
}

int	main(int argc, char **argv)
{
	dinner_t	d;
	int			i;
	
	init_dinner(&d);
	i = 0;
	//isdigit or input check or error return from atod	
	if (argc == 5 || argc == 6)
	{
		if (argv[1] != NULL)
			d.nb_phil = ft_atoi(argv[1]);
		if (argv[2] != NULL)
			d.time_die = ft_atod(argv[2]);
		if (argv[3] != NULL)
			d.time_eat = ft_atod(argv[3]);
		if (argv[4] != NULL)
			d.time_sleep = ft_atod(argv[4]);
		if (argc == 6)
			if (argv[5] != NULL)
				d.eating_times = ft_atoi(argv[5]);
	}
	else 
		write (1, "wrong input\n", 12);
	//this does not belong here vielleicht doch
	d.mutex_chops = malloc((d.nb_phil * sizeof(pthread_mutex_t)));
	while (i < d.nb_phil)
	{
		pthread_mutex_init(&d.mutex_chops[i], NULL);
		i ++;
	}
	d.chops = malloc((d.nb_phil * sizeof(int)));
	memset(d.chops, 0, d.nb_phil * sizeof(int)); //check forb func
	create_threads(d.nb_phil, &d);
	return (0);
}
