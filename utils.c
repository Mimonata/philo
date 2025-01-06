/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 16:43:38 by spitul            #+#    #+#             */
/*   Updated: 2024/12/17 18:31:33 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_space(char *s)
{
	return (*s == ' ' || (*s >= 9 && *s <= 13));
}

int	is_digit(const char s)
{
	return (s >= '0' && s <= '9');
}

int	input_error(int err)
{
	if (err == 1)
		printf("Arguments can only be positive numbers\n");
	else if (err == 2)
		printf("Arguments can only be numbers\n");
	else if (err == 3)
		printf("Arguments must not be bigger than INT_MAX\n");
	return (0);
}

long	ft_atol_phil(const char *nptr)
{
	int i;
	long r;

	i = 0;
	r = 0;
	while (((nptr[i] >= 9) && (nptr[i] <= 13)) || (nptr[i] == 32)
		|| nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			return (input_error(1));
		i++;
	}
	while (nptr[i])
	{
		if (is_digit(nptr[i]) == 0)
		{
			return (input_error(2));
		}
		else
			r = ((r * 10) + (nptr[i] - 48));
		i++;
	}
	if (r > INT_MAX)
		return (input_error(3));
	return (r);
}

	void print_error(char *s)
	{
		// cleanup
	}

	// void	printing(philo_t *f, int state, long time)
	// {

	// }

	double ft_atod(char *s)
	{
		long double c;
		double fract_n;
		double power;
		int sign;

		sign = 1;
		power = 1;
		c = 0;
		fract_n = 0;
		while (*s == 32 || (*s >= 9 && *s <= 13))
			s++;
		while (*s == '+' || *s == '-')
			if (*s++ == '-')
				sign = -sign;
		while (*s != '.' && (*s >= 48 && *s <= 57) && *s)
			c = c * 10 + (*s++ - 48);
		if (*s == '.')
			s++;
		while ((*s >= 48 && *s <= 57) && *s)
		{
			power /= 10;
			fract_n = fract_n + (*s++ - 48) * power;
		}
		return ((c + fract_n) * sign);
	}
