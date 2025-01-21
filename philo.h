/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 21:17:50 by spitul            #+#    #+#             */
/*   Updated: 2025/01/21 19:50:11 by spitul           ###   ########.fr       */
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
	bool			mon_ready;
	long			(*states)[2];
	pthread_mutex_t	*mtx_forks;
	pthread_mutex_t	*mtx_states;
	pthread_mutex_t	mtx_print;
	pthread_mutex_t	mtx_end;
}					t_dinner;

typedef struct philo_s
{
	int				index;
	int				left;
	t_dinner		*dinner_data;
}					t_philo;

long				ft_atol_phil(const char *nptr);
void				init_dinner(t_dinner *d);
int					parse_input(int argc, char **argv, t_dinner *d);
int					allocate_resources(t_dinner *d);
int					prepare_din_sim(int nb_phil, t_dinner *d);

void				thinking(t_philo *f);
void				sleeping(t_philo *f);
int					dinner_synchro(t_philo *f, int right);

void				printing(t_philo *f, int state);
void				set_long(t_philo *f, long *var, long value);
long				get_long(t_philo *f, long *var);
bool				get_bool(pthread_mutex_t *mtx, bool *value);
void				set_bool(pthread_mutex_t *mtx, bool *var, bool val);
int					print_error(char *msg);
int					cleanup_din(t_dinner *d, char *msg);
void				cleanup_th(t_dinner *d, t_philo *f, pthread_t *th, int i);

long				timestamp(void);

int					start_phil_threads(t_dinner *d, t_philo *f, pthread_t *th);
void				init_philo_th(t_philo *f, t_dinner *d, int i);
void				wait_all_threads(t_dinner *d);
void				*start_routine(void *arg);

void				check_death(t_dinner *m);
int					check_meals(t_dinner *m);
int					start_monitor(t_dinner *d);
void				*create_monitor(void *arg);

#endif
