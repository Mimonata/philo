/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 15:45:33 by spitul            #+#    #+#             */
/*   Updated: 2024/06/16 17:14:32 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	int	nb_phil;
	philo_t	fil;
	
	nb_phil = 0;
	if (argc == 5 || argc == 6)
	{
		if (argv[1] != NULL)
			nb_phil = atoi(argv[1]);
		if (argv[2] != NULL)
			fil.time_die = atod(argv[2]);
		if (argv[3] != NULL)
			fil.time_eat = atod(argv[3]);
		if (argv[4] != NULL)
			fil.time_sleep = atod(argv[4]);
		if (argc == 6)
			if (argv[5] != NULL)
				fil.eating_times = atoi(argv[5]);
	}
	else 
		write (1, "wrong input", 11);
	create_threads(nb_phil);
	return (0);
}
