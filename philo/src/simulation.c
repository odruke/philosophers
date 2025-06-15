
#include "philo.h"

// void	mutex_is_locked(pthread_mutex_t *mutex, t_philo *philo, char *file, int line)
// {
// 	if (pthread_mutex_trylock(mutex) == EBUSY)
// 	{
//     	fprintf(stderr, ">>> philo[%d] mutex still locked at %s:%i!\n", philo->id, file, line);
// 		return ;
// 	}
// 	pthread_mutex_unlock(mutex);
// }
void	*single_philo(void *value)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)value;
	data = philo->data;
	set_long(&philo->philo_mutex, &philo->last_meal_time, get_time(MILISECOND));
	// mutex_is_locked(&philo->philo_mutex, philo, __FILE__, __LINE__);
	set_long(&data->data_mutex, &data->n_running_philos, (data->n_running_philos + 1));
	print_status(philo, TAKE_FORK);
	better_usleep((data->tt_die * 1000), data);
	print_status(philo, DIED);
	set_bool(&data->data_mutex, &data->end_sim, true);
	return (NULL);
}

void	eat_routine(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	safe_mutex_handle(&philo->fork[0]->fork, (t_thrhandle){LOCK, __FILE__, __LINE__});
	print_status(philo, TAKE_FORK);
	safe_mutex_handle(&philo->fork[1]->fork, (t_thrhandle){LOCK, __FILE__, __LINE__});
	print_status(philo, TAKE_FORK);
	set_long(&philo->philo_mutex, &philo->last_meal_time, get_time(MILISECOND));
	// mutex_is_locked(&philo->philo_mutex, philo, __FILE__, __LINE__);
	// printf("last_meal_time in philo n:%i is %ld\n", philo->id, philo->last_meal_time);
	// printf("philo n: %i eat at:%ld\n", philo->id, get_time(MILISECOND));
	philo->meals_counter++;
	print_status(philo, EATING);
	better_usleep(data->tt_eat * 1000, data);
	if (data->limit_meals > 0 && philo->meals_counter == data->limit_meals)
	{
		set_bool(&philo->philo_mutex, &philo->is_full, true);
		// mutex_is_locked(&philo->philo_mutex, philo, __FILE__, __LINE__);
	}
	safe_mutex_handle(&philo->fork[0]->fork, (t_thrhandle){UNLOCK, __FILE__, __LINE__});
	print_status(philo, LEAVE_FORK);
	safe_mutex_handle(&philo->fork[1]->fork, (t_thrhandle){UNLOCK, __FILE__, __LINE__});
	print_status(philo, LEAVE_FORK);
	// printf("philo:%i eat finished\n", philo->id);
}

void	sleep_routine(t_philo *philo)
{
	// printf("philo:%i entered sleep\n", philo
	print_status(philo, SLEEPING);
	better_usleep(philo->data->tt_sleep * 1000, philo->data);
	// printf("philo:%i exited sleep\n", philo->id);

}

void	think_routine(t_philo *philo)//to do
{
	// printf("philo:%i entered think\n", philo->id);
	t_data	*data;
	long	tt_think;

	data = philo->data;
	tt_think = data->tt_eat / 2;
	if (data->nb_philos % 2)
		better_usleep(tt_think * 1000, data);
	print_status(philo, THINKING);
	// printf("philo:%i exited think\n", philo->id);
}


void	*simulation(void *value)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)value;
	data = philo->data;
	while (!get_bool(&data->data_mutex, &data->philos_ready))
		usleep(0);
	set_long(&philo->philo_mutex, &philo->last_meal_time, data->start_sim);
	set_long(&data->data_mutex, &data->n_running_philos, (data->n_running_philos + 1));
	while (!get_bool(&data->data_mutex, &data->end_sim))
	{
		if (get_bool(&philo->philo_mutex, &philo->is_full))
			break ;
		//eat
		eat_routine(philo);
		if (get_bool(&philo->philo_mutex, &philo->is_full))
			break ;
		//sleep
		sleep_routine(philo);
		//think
		think_routine(philo);
		// printf("philo: %i simulation loop finished\n", philo->id);
	}
	// mutex_is_locked(&philo->philo_mutex, philo, __FILE__, __LINE__);
	// printf("philo: %i simulation finished\n", philo->id);
	return (NULL);
}

bool	philo_died(t_philo *philo)
{
	long	elapsed;

	if (get_bool(&philo->philo_mutex, &philo->is_full))
		return (false);
	safe_mutex_handle(&philo->philo_mutex, (t_thrhandle){LOCK, __FILE__, __LINE__});
	elapsed = get_time(MILISECOND) - philo->last_meal_time;
	// printf("time elapsed in philo died %ld\n", elapsed);
	// printf("last_meal_time in philo n: %i = %ld\n", philo->id, philo->last_meal_time);
	safe_mutex_handle(&philo->philo_mutex, (t_thrhandle){UNLOCK, __FILE__, __LINE__});
	if (elapsed > philo->data->tt_die)
		return (true);
	return (false);
}

void	*death_monitor(void *value)
{
	t_data	*data;
	int		i;

	data = (t_data *)value;
	while (!all_philos_running(&data->data_mutex, &data->n_running_philos, data->nb_philos))
		usleep(0) ;
	while (!get_bool(&data->data_mutex, &data->end_sim))
	{
		i = -1;
		while (++i < data->nb_philos && !get_bool(&data->data_mutex, &data->end_sim))
		{
			if (philo_died(&data->philos[i]))
			{
				print_status(&data->philos[i], DIED);
				set_bool(&data->data_mutex, &data->end_sim, true);
			}
		}
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
		safe_thread_handle(&data->philos[0].thread_id, single_philo, &data->philos[0], (t_thrhandle){CREATE, __FILE__, __LINE__});
	else
	{
		while (++i < data->nb_philos)
			safe_thread_handle(&data->philos[i].thread_id, simulation, &data->philos[i], (t_thrhandle){CREATE, __FILE__, __LINE__});
	}
	safe_thread_handle(&data->monitor, death_monitor, data, (t_thrhandle){CREATE, __FILE__, __LINE__});
	set_long(&data->data_mutex, &data->start_sim, get_time(MILISECOND));
	set_bool(&data->data_mutex, &data->philos_ready, true);
	// data->start_sim = get_time(MILISECOND);
	i = -1;
	while (++i < data->nb_philos)
		safe_thread_handle(&data->philos[i].thread_id, NULL, NULL, (t_thrhandle){JOIN, __FILE__, __LINE__});
	set_bool(&data->data_mutex, &data->end_sim, true);
	safe_thread_handle(&data->monitor, NULL, NULL, (t_thrhandle){JOIN, __FILE__, __LINE__});
}
