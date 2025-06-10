#include "philo.h"
void	single_philo(t_data *data)
{
	(void)data;
	return ;
}

void	eat_routine(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	safe_mutex_handle(&philo->fork[0].fork, (t_thrhandle){LOCK, __FILE__, __LINE__});
	print_status(philo, TAKE_FORK);
	safe_mutex_handle(&philo->fork[1].fork, (t_thrhandle){LOCK, __FILE__, __LINE__});
	print_status(philo, TAKE_FORK);
	set_long(&philo->philo_mutex, &philo->last_meal_time, get_time(MILISECOND));
	philo->meals_counter++;
	print_status(philo, EATING);
	better_usleep(data->tt_eat, data);
	if (data->limit_meals > 0 && philo->meals_counter == data->limit_meals)
		set_bool(&philo->philo_mutex, &philo->is_full, true);
	safe_mutex_handle(&philo->fork[0].fork, (t_thrhandle){UNLOCK, __FILE__, __LINE__});
	safe_mutex_handle(&philo->fork[1].fork, (t_thrhandle){UNLOCK, __FILE__, __LINE__});
}

void	sleep_routine(t_philo *philo)
{
	print_status(philo, SLEEPING);
	better_usleep(philo->data->tt_sleep, philo->data);
}

void	think_routine(t_philo *philo)//to do
{
	print_status(philo, THINKING);
}

void	*simulation(void *value)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)value;
	data = philo->data;
	while (!get_bool(&data->data_mutex, data->philos_ready))
		;
	while (!get_bool(&data->data_mutex, data->end_sim))
	{
		if (get_bool(&data->data_mutex, philo->is_full))
			break ;
		//eat
		eat_routine(philo);
		//sleep
		sleep_routine(philo);
		think_routine(philo);
		//think
	}
	return (NULL);
}
void	start_simulation(t_data *data)
{
	int	i;

	i = -1;
	if (!data->limit_meals)
		return ;
	else if (data->nb_philos == 1)
		single_philo(data);
	else
	{
		while (++i < data->nb_philos)
			safe_thread_handle(&data->philos[i].thread_id, simulation, &data->philos[i], (t_thrhandle){CREATE, __FILE__, __LINE__});
	}
	data->start_sim = get_time(MILISECOND);
	set_bool(&data->data_mutex, &data->philos_ready, true);
	i = -1;
	while (++i < data->nb_philos)
		safe_thread_handle(&data->philos[i].thread_id, NULL, NULL, (t_thrhandle){JOIN, __FILE__, __LINE__});

}
