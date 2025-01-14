/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 21:17:50 by spitul            #+#    #+#             */
/*   Updated: 2025/01/14 18:27:37 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

# define LAST_EAT 0
# define MEALS_EATEN 1

# define EATING 2
# define SLEEPING 3
# define THINKING 4
# define TAKES_LEFTFORK 5
# define TAKES_RIGHTFORK 6
# define DIED 7

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
	bool			end_din;
	bool			all_ready;
	//{last_eat, meals_eaten}
	long (*states)[2];
	pthread_mutex_t	*mtx_forks;
	pthread_mutex_t	*mtx_states;
	pthread_mutex_t	mtx_print;
	pthread_mutex_t	mtx_end;
}					dinner_t;

typedef struct philo_s
{
	int				index;
	int				left;
	//long			last_eat;
	dinner_t		*dinner_data;
}					philo_t;

void				init_dinner(dinner_t *d);
int					parse_input(int argc, char **argv, dinner_t *d);
int					allocate_resources(dinner_t *d);
int					prepare_din_sim(int nb_phil, dinner_t *d);

int					dinner_synchro(philo_t *f, int right);

long				ft_atol_phil(const char *nptr);
void				printing(philo_t *f, int state);
void				set_long(philo_t *f, long *var, long value);
bool				get_bool(pthread_mutex_t mtx, bool var);
void				set_bool(pthread_mutex_t mtx, bool *var, bool val);
int					print_error(char *msg);
int					cleanup_din(dinner_t *d, char *msg);
void				cleanup_th(dinner_t *d, philo_t *f, pthread_t *th, int i);

long				timestamp(void);

int					start_phil_threads(dinner_t *d, philo_t *f, pthread_t *th);
void				init_philo_th(philo_t *f, dinner_t *d, int i);
void				wait_all_threads(dinner_t *d);
void				*start_routine(void *arg);

void				check_death(dinner_t *m);
int					check_meals(dinner_t *m);
int					start_monitor(dinner_t *d);
void				*create_monitor(void *arg);

#endif
