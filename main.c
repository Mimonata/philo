/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 15:45:33 by spitul            #+#    #+#             */
/*   Updated: 2024/06/20 13:23:53 by spitul           ###   ########.fr       */
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
			nb_phil = ft_atoi(argv[1]);
		if (argv[2] != NULL)
			fil.time_die = ft_atod(argv[2]);
		if (argv[3] != NULL)
			fil.time_eat = ft_atod(argv[3]);
		if (argv[4] != NULL)
			fil.time_sleep = ft_atod(argv[4]);
		if (argc == 6)
			if (argv[5] != NULL)
				fil.eating_times = ft_atoi(argv[5]);
	}
	else 
		write (1, "wrong input", 11);
	create_threads(nb_phil, &fil);
	return (0);
}
