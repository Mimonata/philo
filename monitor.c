/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 16:22:04 by spitul            #+#    #+#             */
/*   Updated: 2024/12/16 20:02:54 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	check_death(dinner_t *m)
{
	int			i;

	i = 1;
	pthread_mutex_lock(&m->mutex_states);
	while (i <= m->nb_phil)
	{
		if (timestamp() - m->states[i][LAST_EAT] > m->time_die)
		{
			m->one_dead = 1;
			printf("\033[1;31m%ld %d has died\033[0m\n", timestamp()
				- m->start_time, i);
			break ;
		}
		i++;
	}
	pthread_mutex_unlock(&m->mutex_states);
}

int	check_meals(dinner_t *m)
{
	int			i;
	int			fin;

	i = 1;
	fin = 0;
	pthread_mutex_lock(&m->mutex_states);
	while (i <= m->nb_phil && fin < m->nb_phil)
	{
		if (m->states[i][MEALS_EATEN] >= m->eating_times)
			fin++;
		i++;
	}
	pthread_mutex_unlock(&m->mutex_states);
	if (fin == m->nb_phil)
	{
		// mutex
		m->one_dead = 1;
		return (1);
	}
	return (0);
}

void	*start_monitor(void *arg)
{
	dinner_t	*m;

	m = (dinner_t *)arg;
	// usleep(1000);
	while (m->one_dead == 0)
	{
		if (m->one_dead == 0)
			check_death(m);
		else
			break;
		if (m->eating_times != -1 && m->one_dead == 0)
			check_meals(m);
		else
			
		usleep(1000);
	}
	return ((void *)m);
}

void	create_monitor(dinner_t *d)
{
	pthread_t	*mh;
	dinner_t	*m;
	
	mh = malloc(sizeof(pthread_t));
	if (!mh)
	//error
	if (pthread_create(mh, NULL, &start_monitor, &d)!= 0)
	{
		printf("**Cannot create monitor**\n");
		return ;
	}
	pthread_join(m, NULL);
}
