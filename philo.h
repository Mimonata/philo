/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 21:17:50 by spitul            #+#    #+#             */
/*   Updated: 2024/12/03 06:50:06 by spitul           ###   ########.fr       */
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

# define	EATING 2
# define	LAST_EAT 0
# define	MEALS_EATEN 1

typedef struct	dinner_s
{
	int		nb_phil;
	long	time_die;
	long	time_eat;
	long	time_sleep;
	int		eating_times;
	long	start_time;		
	pthread_mutex_t		*mutex_chops;
	int		*chops; // dont think this is needed but lets see
	int		one_dead;
	long	(*states)[2]; //{last_eat, meals_eaten}
}				dinner_t;

typedef struct philo_s
{
	int		index;
	int		left;
	//int		meals_nb;
	long	last_eat;
	dinner_t	*dinner_data;
}				philo_t;

int	create_threads(int nb_phil, dinner_t *d);

int	take_forks(philo_t *f, int right);

int	ft_atoi(const char *nptr);
double	ft_atod(char *s);

#endif
