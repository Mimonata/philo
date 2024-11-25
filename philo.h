/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 21:17:50 by spitul            #+#    #+#             */
/*   Updated: 2024/07/02 19:43:16 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct	dinner_s
{
	int		nb_phil;
	double	time_die;
	double	time_eat;
	double	time_sleep;
	int		eating_times;		
	pthread_mutex_t		*mutex_chops;
	int		*chops;
}				dinner_t;

typedef struct philo_s
{
	int		index;
	int		left;
	double	last_eat;
	dinner_t	*dinner_data;
}				philo_t;

int	create_threads(int nb_phil, dinner_t *d);

void	take_forks(philo_t *f, long start, int right);

int	ft_atoi(const char *nptr);
double	ft_atod(char *s);

#endif