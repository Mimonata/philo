/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threading.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 16:49:28 by spitul            #+#    #+#             */
/*   Updated: 2024/06/16 18:21:40 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*start_routine(void *arg)
{
	philo_t	*fil[]
}

int create_threads(int nb_phil)
{
	pthread_t	*th;
	int	i;

	i = 0;
	while (i <= nb_phil)
	{
		if (pthread_create(th[i], NULL, &start_routine, &i) == -1)
		{
			write(1, "can't create thread", 18);
			return (1);
		}
		i ++;	
	}
	return (0);
}
