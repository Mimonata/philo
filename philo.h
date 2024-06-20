/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 21:17:50 by spitul            #+#    #+#             */
/*   Updated: 2024/06/20 13:46:17 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct	philo_s
{
	int		index;
	double	time_die;
	double	time_eat;
	double	time_sleep;
	int		eating_times;
	int		forks[2];		
}				philo_t;

int create_threads(int nb_phil, philo_t *f);

int	ft_atoi(const char *nptr);
double	ft_atod(char *s);

#endif