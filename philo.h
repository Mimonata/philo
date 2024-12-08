/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 21:17:50 by spitul            #+#    #+#             */
/*   Updated: 2024/12/08 17:18:37 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

# define LAST_EAT 0
# define MEALS_EATEN 1

# define EATING 2
# define SlEEPING 3
# define THINKING 4

typedef struct dinner_s
{
	int				nb_phil;
	long			time_die;
	long			time_eat;
	long			time_sleep;
	int				eating_times;
	long			start_time;
	int				*chops;
	int				one_dead;
						//{last_eat, meals_eaten}
	long			(*states)[2];
	pthread_mutex_t	*mutex_chops;
	pthread_mutex_t	mutex_states;
	pthread_mutex_t	mutex_print;
}					dinner_t;

typedef struct philo_s
{
	int				index;
	int				left;
	// int		meals_nb;
	long			last_eat;
	dinner_t		*dinner_data;
}					philo_t;

int					create_threads(int nb_phil, dinner_t *d);

int					take_forks(philo_t *f, int right);

int					ft_atoi(const char *nptr);
double				ft_atod(char *s);

long				timestamp(void);

void				check_death(philo_t *m);
int					check_meals(philo_t *m);
void				*start_monitor(void *arg);

#endif
