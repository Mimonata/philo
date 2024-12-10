/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 16:22:04 by spitul            #+#    #+#             */
/*   Updated: 2024/12/10 06:30:59 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	check_death(philo_t *m)
{
	int			i;
	dinner_t	*din;

	din = m->dinner_data;
	i = 1;
	pthread_mutex_lock(&din->mutex_states);
	while (i <= din->nb_phil)
	{
		if (timestamp() - din->states[i][LAST_EAT] > din->time_die)
		{
			din->one_dead = 1;
			printf("\033[1;31m%ld %d has died\033[0m\n", timestamp()
				- din->start_time, i);
			break ;
		}
		i++;
	}
	pthread_mutex_unlock(&din->mutex_states);
}

int	check_meals(philo_t *m)
{
	int			i;
	int			fin;
	dinner_t	*din;

	i = 1;
	fin = 0;
	din = m->dinner_data;
	pthread_mutex_lock(&din->mutex_states);
	while (i <= din->nb_phil && fin < din->nb_phil)
	{
		if (din->states[i][MEALS_EATEN] >= din->eating_times)
			fin++;
		i++;
	}
	pthread_mutex_unlock(&din->mutex_states);
	if (fin == din->nb_phil)
	{
		// mutex
		din->one_dead = 1;
		return (1);
	}
	return (0);
}

void	create_monitor(void)
{
	pthread_t	*mh;
	dinner_t	*m;
	
	m = malloc(sizeof(dinner_t));
	if (!m)
	//error
	mh = malloc(sizeof(pthread_t));
	if (!mh)
	//error
}

void	*start_monitor(void *arg)
{
	philo_t	*m;

	m = (philo_t *)arg;
	// usleep(1000);
	while (m->dinner_data->one_dead == 0)
	{
		check_death(m);
		check_meals(m); // rethink
	}
	return ((void *)m);
}
