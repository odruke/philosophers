/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odruke-s <odruke-s@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 22:33:29 by odruke-s          #+#    #+#             */
/*   Updated: 2025/06/18 22:33:51 by odruke-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_table(void **table)
{
	int	i;

	i = -1;
	while (table[++i])
		free(table[i]);
	free(table);
	table = NULL;
}

static void	destroy_extra_mutexes(t_data *data)
{
	safe_mutex_handle(&data->data_mutex,
		(t_thrhandle){DESTROY, __FILE__, __LINE__});
	safe_mutex_handle(&data->print_mutex,
		(t_thrhandle){DESTROY, __FILE__, __LINE__});
}

void	free_data(t_data *data)
{
	int	i;

	if (data)
	{
		if (data->philos)
		{
			i = -1;
			while (++i < data->nb_philos)
				safe_mutex_handle(&data->philos[i].philo_mutex,
					(t_thrhandle){DESTROY, __FILE__, __LINE__});
			free(data->philos);
		}
		if (data->forks)
		{
			i = -1;
			while (++i < data->nb_philos)
				safe_mutex_handle(&data->forks[i].fork,
					(t_thrhandle){DESTROY, __FILE__, __LINE__});
			free(data->forks);
		}
		destroy_extra_mutexes(data);
		free(data);
	}
}
