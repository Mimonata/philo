/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threading.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 16:49:28 by spitul            #+#    #+#             */
/*   Updated: 2024/06/20 14:51:07 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*start_routine(void *arg)
{
	philo_t	*fil;
	fil = (philo_t *)arg;

	printf("this is thread %d \n", fil->index);
	printf("fil.time to die %f\n", fil->time_die);
	return (fil);
}

int create_threads(int nb_phil, philo_t *fil)
{
	pthread_t	*th;
	philo_t		*f;
	int	i;

	th = malloc(nb_phil * sizeof(pthread_t));
	f = malloc(nb_phil * sizeof(philo_t));
	i = 0;
	while (i < nb_phil)
	{
		f = fil;
		f[i]->index = i;
		if (pthread_create(&th[i], NULL, &start_routine, &f[i]) != 0)
		{
			write(1, "can't create thread", 18);
			free(f[i]);
			free(th[i]);
			return (1);
		}
		
		
		i ++;	
	}
	return (0);
}
