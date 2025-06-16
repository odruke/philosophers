/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   main.c                                              :+:    :+:           */
/*                                                      +:+                   */
/*   By: odruke-s <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/06/16 13:31:35 by odruke-s       #+#    #+#                */
/*   Updated: 2025/06/16 13:32:00 by odruke-s       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_data	*recover_data_address(t_data *data)
{
	static t_data	*stock_data;

	if (data)
		stock_data = data;
	else
		data = stock_data;
	return (stock_data);
}

int	main(int ac, char **av)
{
	t_data	*data;

	input_validation(ac, av);
	data = (t_data *)safe_malloc(sizeof(t_data), __FILE__, __LINE__);
	recover_data_address(data);
	fill_data(data);
	parsing(data, av);
	init_data(data);
	start_simulation(data);
	free_data(data);
	return (EXIT_SUCCESS);
}
