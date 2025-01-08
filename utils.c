/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 18:05:50 by spitul            #+#    #+#             */
/*   Updated: 2025/01/08 20:27:49 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	print_error(char *msg)
{
	if (msg)
		printf("%s\n", msg);
	return (0);
}

void	printing(philo_t *f, int state, long time)
{
	dinner_t	*din;

	din = f->dinner_data;
	pthread_mutex_lock(&din->mtx_print);
	if (state == EATING && din->one_dead == 0)
		printf("\x1b[38;200;120;0;255m%ld %d is eating\x1b[0m\n", time
			- din->start_time, f->index);
	else if (state == SLEEPING && din->one_dead == 0)
		printf("\x1b[38;112;120;0;255m%ld %d is sleeping\x1b[0m\n", time
			- din->start_time, f->index);
	else if (state == THINKING && din->one_dead == 0)
		printf("\x1b[38;2;120;0;255m%ld %d is thinking\x1b[0m\n", time
			- din->start_time, f->index);
	else if ((state == TAKES_LEFFORK || state == TAKES_RIGHTFORK)
		&& din->one_dead == 0)
		printf("\x1b[38;182;120;0;255m%ld %d has taken a fork\x1b[0m\n", time
			- din->start_time, f->index);
	pthread_mutex_unlock(&din->mtx_print);
}

int	cleanup_din(dinner_t *d, char *msg)
{
	int	i;
	
	i = 0;
	if (d->mtx_chops)
	{
		while (i < d->nb_phil)
		{
			pthread_mutex_destroy(&d->mtx_chops[i]);
			i ++;
		}
		free (d->mtx_chops);
	}
	pthread_mutex_destroy(&d->mtx_print);
	pthread_mutex_destroy(&d->mtx_states);
	if (d->states)
		free (d->states);
	if (d->chops)
		free (d->chops);
	if (msg) //einfach nur ein printf, wenn nur am Anfang verwendet - TO LOOK AT
		printf("%s\n", msg);
	return (0);
}

void	cleanup_th(dinner_t *d, philo_t *f, pthread_t *th, int i)
{
	if (i > 0)
	{
		i --;
		while (th && i >= 0)
		{
			pthread_join(th[i], NULL);
			i --;
		}
	}
	if (th)
		free (th);
	if (f)
		free (f);
	cleanup_din(d, NULL);
	//return (0);
}

int	create_phil_threads(dinner_t *d, philo_t *f, pthread_t *th)
{
	int	i;

	i = 0;
	while (i < d->nb_phil)
	{
		init_philo_th(&f[i], d, i);
		if (pthread_create(&th[i], NULL, &start_routine, &f[i]) != 0)
		{
			cleanup_th(d, f, th, i);
			return (print_error("Failure creating thread"));
		}
		i++;
	}
	return (1);
}
