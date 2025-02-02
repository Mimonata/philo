/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 15:45:33 by spitul            #+#    #+#             */
/*   Updated: 2025/01/21 19:41:48 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_dinner	d;
	int			sim;

	init_dinner(&d);
	if (argc == 5 || argc == 6)
	{
		sim = parse_input(argc, argv, &d);
	}
	else
		return (!print_error("Wrong input"));
	if (sim == 1)
	{
		if (allocate_resources(&d))
			prepare_din_sim(d.nb_phil, &d);
	}
	return (0);
}
