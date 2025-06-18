/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odruke-s <odruke-s@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 22:30:04 by odruke-s          #+#    #+#             */
/*   Updated: 2025/06/18 22:30:06 by odruke-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	all_philos_are_full(t_data *data)
{
	t_philo	*philos;
	int		i;

	philos = data->philos;
	i = -1;
	while (++i < data->nb_philos)
		if (!get_bool(&philos[i].philo_mutex, &philos[i].is_full))
			return (false);
	return (true);
}

bool	all_philos_running(pthread_mutex_t *mutex,
			long *philos_runing, long nb_philos)
{
	bool	res;

	res = false;
	safe_mutex_handle(mutex, (t_thrhandle){LOCK, __FILE__, __LINE__});
	if (*philos_runing == nb_philos)
		res = true;
	safe_mutex_handle(mutex, (t_thrhandle){UNLOCK, __FILE__, __LINE__});
	return (res);
}

void	launch_philos(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->nb_philos)
		safe_thread_handle(&data->philos[i].thread_id,
			simulation, &data->philos[i],
			(t_thrhandle){CREATE, __FILE__, __LINE__});
}
