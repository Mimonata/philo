/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 21:17:50 by spitul            #+#    #+#             */
/*   Updated: 2024/06/16 18:12:21 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct	philo_s
{
	double	time_die;
	double	time_eat;
	double	time_sleep;
	int		eating_times;
	int		forks[2];		
}				philo_t;

int create_threads(int nb_phil);

int	ft_atoi(const char *nptr);
double	atod(char *s);

#endif