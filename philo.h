/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 21:17:50 by spitul            #+#    #+#             */
/*   Updated: 2024/06/21 16:49:29 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct philo_s
{
	int		index;
	int		chopsticks[2];
	long long last_eat;
}				philo_t;

typedef struct	dinner_s
{
	int		nb_phil;
	double	time_die;
	double	time_eat;
	double	time_sleep;
	int		eating_times;		
}				dinner_t;

int	create_threads(int nb_phil, dinner_t *d);

int	ft_atoi(const char *nptr);
double	ft_atod(char *s);

#endif