/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 15:45:33 by spitul            #+#    #+#             */
/*   Updated: 2024/06/24 19:16:42 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_dinner(dinner_t d)
{
	d.nb_phil = 0;
	d.eating_times = 0.0;
	d.time_die = 0.0;
	d.time_eat = 0.0;
	d.time_sleep = 0.0;
	d.forks = malloc((d.nb_phil * sizeof(int)) + 1);
	memset(d.forks, '0', d.nb_phil);
}

int	main(int argc, char **argv)
{
	dinner_t	d;
	
	init_dinner(d);
	//isdigit or input check or error return from atodi	
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
		write (1, "wrong input", 11);
	create_threads(d.nb_phil, &d);
	return (0);
}
