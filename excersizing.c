#include "philo.h"

void	*routine()

int main()
{
	pthread_t	th[2];

	if (pthread_create(th[0], NULL, routine, NULL) != 0)
		return (1);
	if (pthread_create(th[1], NULL, routine, NULL) != 0)
		return (2);
}