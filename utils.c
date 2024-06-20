/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 16:43:38 by spitul            #+#    #+#             */
/*   Updated: 2024/06/20 12:44:37 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

double	ft_atod(char *s)
{
	long double	c;
	double		fract_n;
	double		power;
	int			sign;

	sign = 1;
	power = 1;
	c = 0;
	fract_n = 0;
	while (*s == 32 || (*s >= 9 && *s <= 13))
		s ++;
	while (*s == '+' || *s == '-')
		if (*s ++ == '-')
			sign = -sign;
	while (*s != '.' && (*s >= 48 && *s <= 57) && *s)
		c = c * 10 + (*s ++ - 48);
	if (*s == '.')
		s ++;
	while ((*s >= 48 && *s <= 57) && *s)
	{
		power /= 10;
		fract_n = fract_n + (*s ++ - 48) * power;
	}
	return ((c + fract_n) * sign);
}

int	ft_atoi(const char *nptr)
{
	int	i;
	int	s;
	int	r;

	i = 0;
	s = 1;
	r = 0;
	while (((nptr[i] >= 9) && (nptr[i] <= 13)) || (nptr[i] == 32))
		i ++;
	if (nptr[i] == '-')
	{
		s = -1;
		i ++;
	}
	else if (nptr[i] == '+')
		i ++;
	while (((nptr[i] >= '0') && (nptr[i] <= '9')) && (nptr[i]))
	{
		r = ((r * 10) + (nptr[i] - 48));
		i ++;
	}
	return (r * s);
}
