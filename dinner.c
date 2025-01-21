/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 19:29:43 by spitul            #+#    #+#             */
/*   Updated: 2025/01/21 19:54:33 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	thinking(t_philo *f)
{
	long		time_think;
	t_dinner	*din;

	din = f->dinner_data;
	time_think = 0;
	if (!get_bool(&f->dinner_data->mtx_end, &f->dinner_data->end_din))
		printing(f, THINKING);
	if (f->dinner_data->nb_phil % 2 == 1)
	{
		time_think = (din->time_eat * 2) - din->time_sleep;
		if (time_think < 0)
			time_think = 0;
		usleep(time_think * 3);
	}
}

void	sleeping(t_philo *f)
{
	if (!get_bool(&f->dinner_data->mtx_end, &f->dinner_data->end_din))
		printing(f, SLEEPING);
	usleep(f->dinner_data->time_sleep * 1000);
	if (get_bool(&f->dinner_data->mtx_end, &f->dinner_data->end_din))
		return ;
}

static void	eating(t_philo *f, int right)
{
	printing(f, EATING);
	set_long(f, &f->dinner_data->states[right][LAST_EAT], timestamp());
	set_long(f, &f->dinner_data->states[right][MEALS_EATEN],
		f->dinner_data->states[right][MEALS_EATEN] + 1);
	usleep(f->dinner_data->time_eat * 1000);
	if (get_bool(&f->dinner_data->mtx_end, &f->dinner_data->end_din))
		return ;
}

static int	grab_forks(t_philo *f, int fork1, int fork2)
{
	t_dinner	*din;

	din = f->dinner_data;
	if (!get_bool(&din->mtx_end, &din->end_din) && (timestamp() - get_long(f,
				&din->states[f->index - 1][LAST_EAT]) < din->time_die))
	{
		pthread_mutex_lock(&din->mtx_forks[fork1]);
		printing(f, TAKES_LEFTFORK);
		pthread_mutex_lock(&din->mtx_forks[fork2]);
		printing(f, TAKES_RIGHTFORK);
		eating(f, f->index - 1);
		pthread_mutex_unlock(&din->mtx_forks[fork1]);
		pthread_mutex_unlock(&din->mtx_forks[fork2]);
		return (1);
	}
	return (0);
}

int	dinner_synchro(t_philo *f, int right)
{
	t_dinner	*din;
	int			res;

	din = f->dinner_data;
	res = -1;
	if (get_bool(&din->mtx_end, &din->end_din))
		return (0);
	if (f->index % 2 == 0 && !get_bool(&din->mtx_end, &din->end_din))
		res = grab_forks(f, f->left, right);
	else if (f->index % 2 == 1 && !get_bool(&din->mtx_end, &din->end_din))
		res = grab_forks(f, right, f->left);
	return (res);
}
