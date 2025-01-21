/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_get.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 19:23:11 by spitul            #+#    #+#             */
/*   Updated: 2025/01/21 19:42:38 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_long(t_philo *f, long *var, long value)
{
	pthread_mutex_lock(&f->dinner_data->mtx_states[f->index - 1]);
	*var = value;
	pthread_mutex_unlock(&f->dinner_data->mtx_states[f->index - 1]);
}

long	get_long(t_philo *f, long *var)
{
	long	value;

	pthread_mutex_lock(&f->dinner_data->mtx_states[f->index - 1]);
	value = *var;
	pthread_mutex_unlock(&f->dinner_data->mtx_states[f->index - 1]);
	return (value);
}

void	set_bool(pthread_mutex_t *mtx, bool *var, bool val)
{
	pthread_mutex_lock(mtx);
	*var = val;
	pthread_mutex_unlock(mtx);
}

bool	get_bool(pthread_mutex_t *mtx, bool *value)
{
	bool	res;

	pthread_mutex_lock(mtx);
	res = *value;
	pthread_mutex_unlock(mtx);
	return (res);
}
