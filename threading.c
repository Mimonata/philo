/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threading.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 16:49:28 by spitul            #+#    #+#             */
/*   Updated: 2024/06/21 16:51:55 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*start_routine(void *arg)
{
	philo_t	*fil;
	fil = (philo_t *)arg;

	printf("this is thread %d \n", fil->index);
	return (fil);
}

int	create_threads(int nb_phil, dinner_t *d)
{
	pthread_t	*th;
	philo_t		*f;
	int	i;

	th = malloc(nb_phil * sizeof(pthread_t));
	f = malloc(nb_phil * sizeof(philo_t));
	i = 0;
	while (i < nb_phil)
	{
		if (pthread_create(&th[i], NULL, &start_routine, &f[i]) != 0)
		{
			write(1, "can't create thread", 18);
			//free(&f[i]);
			//free(&th[i]);
			return (1);
		}
		usleep(1000);
		//printf("this is i %d\n", i);
		
		i ++;	
	}
	return (0);
}
