/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 18:05:50 by spitul            #+#    #+#             */
/*   Updated: 2025/01/21 18:02:02 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	print_error(char *msg)
{
	if (msg)
		printf("%s\n", msg);
	return (0);
}

void	printing(philo_t *f, int state)
{
	dinner_t	*din;

	din = f->dinner_data;
	pthread_mutex_lock(&din->mtx_print);
	if (state == EATING && !get_bool(&din->mtx_end, &din->end_din))
		printf("\x1b[38;2;120;0;255m%ld %d is eating\x1b[0m\n", timestamp()
			- din->start_time, (f->index));
	else if (state == SLEEPING && !get_bool(&din->mtx_end, &din->end_din))
		printf("%ld %d is sleeping\n", timestamp() - din->start_time,
			(f->index));
	else if (state == THINKING && !get_bool(&din->mtx_end, &din->end_din))
		printf("%ld %d is thinking\n", timestamp() - din->start_time,
			(f->index));
	else if ((state == TAKES_LEFTFORK || state == TAKES_RIGHTFORK)
		&& (!get_bool(&din->mtx_end, &din->end_din)))
		printf("%ld %d has taken a fork\n", timestamp() - din->start_time,
			(f->index));
	else if (state == DIED)
		printf("%ld %d has died\n", timestamp() - din->start_time, (f->index));
	pthread_mutex_unlock(&din->mtx_print);
}

void	clean_mtx_array(pthread_mutex_t *mtx, int nb_phil)
{
	int	i;

	i = 0;
	while (i < nb_phil)
	{
		pthread_mutex_destroy(&mtx[i]);
		i++;
	}
	free(mtx);
}

int	cleanup_din(dinner_t *d, char *msg)
{
	if (d->mtx_forks)
		clean_mtx_array(d->mtx_forks, d->nb_phil);
	if (d->mtx_states)
		clean_mtx_array(d->mtx_states, d->nb_phil);
	pthread_mutex_destroy(&d->mtx_print);
	pthread_mutex_destroy(&d->mtx_end);
	if (d->states)
		free(d->states);
	if (d->chops)
		free(d->chops);
	if (msg) // nur ein printf, wenn nur am Anfang verwendet- TO LOOK AT
		printf("%s\n", msg);
	return (0);
}

void	cleanup_th(dinner_t *d, philo_t *f, pthread_t *th, int n)
{
	int	i;

	i = 0;
	if (n > 0)
	{
		while (th && i < n)
		{
			pthread_join(th[i], NULL);
			i++;
		}
	}
	if (th)
		free(th);
	if (f)
		free(f);
	cleanup_din(d, NULL);
	// return (0);
}

int	start_phil_threads(dinner_t *d, philo_t *f, pthread_t *th)
{
	int	i;

	i = 0;
	d->start_time = timestamp();
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
	//  pthread_mutex_lock(&d->mtx_print);
    // d->all_ready = true;
    // pthread_mutex_unlock(&d->mtx_print);
	set_bool(&d->mtx_print, &d->all_ready, true);
	d->mon_ready = true;
	return (1);
}
