/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                      +:+                   */
/*   By: odruke-s <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/06/16 13:52:51 by odruke-s       #+#    #+#                */
/*   Updated: 2025/06/18 22:15:32 by odruke-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*single_philo(void *value)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)value;
	data = philo->data;
	set_long(&philo->philo_mutex, &philo->last_meal_time,
		get_time(MICROSECOND));
	set_long(&data->data_mutex, &data->n_running_philos,
		(data->n_running_philos + 1));
	print_status(philo, TAKE_FORK);
	better_usleep((data->tt_die * 1000), data);
	print_status(philo, DIED);
	set_bool(&data->data_mutex, &data->end_sim, true);
	return (NULL);
}

void	*simulation(void *value)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)value;
	data = philo->data;
	increment_long(&data->data_mutex, &data->n_running_philos);
	while (!all_philos_running(&data->data_mutex,
			&data->n_running_philos, data->nb_philos))
		usleep(0);
	if ((data->nb_philos % 2))
		if (!(philo->id % 2))
			better_usleep(3000, data);
	while (!get_bool(&data->data_mutex, &data->end_sim))
	{
		eat_routine(philo);
		if (get_bool(&philo->philo_mutex, &philo->is_full))
			break ;
		sleep_routine(philo);
		think_routine(philo);
	}
	return (NULL);
}

bool	philo_died(t_philo *philo)
{
	long	elapsed;

	if (get_bool(&philo->philo_mutex, &philo->is_full))
		return (false);
	if (!get_long(&philo->philo_mutex, &philo->last_meal_time))
		return (false);
	safe_mutex_handle(&philo->philo_mutex,
		(t_thrhandle){LOCK, __FILE__, __LINE__});
	elapsed = get_time(MICROSECOND) - philo->last_meal_time;
	safe_mutex_handle(&philo->philo_mutex,
		(t_thrhandle){UNLOCK, __FILE__, __LINE__});
	if (elapsed > (philo->data->tt_die * 1000))
	{
		print_status(philo, DIED);
		return (true);
	}
	return (false);
}

void	*death_monitor(void *value)
{
	t_data	*data;
	int		i;

	data = (t_data *)value;
	while (!all_philos_running(&data->data_mutex,
			&data->n_running_philos, data->nb_philos))
		usleep(0);
	while (!get_bool(&data->data_mutex, &data->end_sim))
	{
		i = -1;
		while (++i < data->nb_philos)
		{
			if (philo_died(&data->philos[i]))
			{
				set_bool(&data->data_mutex, &data->end_sim, true);
				break ;
			}
		}
		if (all_philos_are_full(data))
		{
			set_bool(&data->data_mutex, &data->end_sim, true);
			break ;
		}
	}
	return (NULL);
}

void	start_simulation(t_data *data)
{
	int	i;

	i = -1;
	data->philos_ready = true;
	if (!data->limit_meals)
		return ;
	else if (data->nb_philos == 1)
		safe_thread_handle(&data->philos[0].thread_id,
			single_philo, &data->philos[0],
			(t_thrhandle){CREATE, __FILE__, __LINE__});
	else
		launch_philos(data);
	set_bool(&data->data_mutex, &data->philos_ready, true);
	safe_thread_handle(&data->monitor, death_monitor, data,
		(t_thrhandle){CREATE, __FILE__, __LINE__});
	i = -1;
	while (++i < data->nb_philos)
		safe_thread_handle(&data->philos[i].thread_id, NULL, NULL,
			(t_thrhandle){JOIN, __FILE__, __LINE__});
	set_bool(&data->data_mutex, &data->end_sim, true);
	safe_thread_handle(&data->monitor, NULL, NULL,
		(t_thrhandle){JOIN, __FILE__, __LINE__});
}
