/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 21:16:02 by spitul            #+#    #+#             */
/*   Updated: 2024/11/18 20:01:30 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	*routine()
{
	printf("test\n");
}


int	main(int argc, char **argv)
{
	pthread_t	th[7];
	pthread_mutex_t	mutex;
	int	i;

	i = 0;
	pthread_mutex_init(&mutex, NULL);
	while (i < 10)
	{
		pthread_create(&th[i], NULL, &routine, NULL);
		i ++;
	}
	i = 0;
	while (i < 7)
	{
		pthread_join(th[i], NULL);
		i ++;
	}
	pthread_mutex_destroy(&mutex);
	return 0;
}
