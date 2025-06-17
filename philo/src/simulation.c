/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   simulation.c                                        :+:    :+:           */
/*                                                      +:+                   */
/*   By: odruke-s <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/06/16 13:52:51 by odruke-s       #+#    #+#                */
/*   Updated: 2025/06/16 13:52:54 by odruke-s       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */


#include "philo.h"

void	*single_philo(void *value)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)value;
	data = philo->data;
	set_long(&philo->philo_mutex, &philo->last_meal_time, get_time(MICROSECOND));
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
	set_long(&philo->philo_mutex, &philo->last_meal_time, get_time(MICROSECOND));
	philo->meals_counter++;
	print_status(philo, EATING);
	better_usleep(data->tt_eat * 1000, data);
	safe_mutex_handle(&philo->fork[0]->fork, (t_thrhandle){UNLOCK, __FILE__, __LINE__});
	print_status(philo, LEAVE_FORK);
	safe_mutex_handle(&philo->fork[1]->fork, (t_thrhandle){UNLOCK, __FILE__, __LINE__});
	print_status(philo, LEAVE_FORK);
	if (data->limit_meals > 0 && philo->meals_counter == data->limit_meals)
		set_bool(&philo->philo_mutex, &philo->is_full, true);
}

void	sleep_routine(t_philo *philo)
{
	print_status(philo, SLEEPING);
	better_usleep(philo->data->tt_sleep * 1000, philo->data);
}

void	think_routine(t_philo *philo)//to test
{
	t_data	*data;
	long	tt_think;
	long	time_until_death;
	// long	last_meal;
	// long	t_die;
	data = philo->data;
    // if (data->nb_philos % 2 == 0)
    //     tt_think = (data->tt_eat + data->tt_sleep) / 2;
    // else
    //     tt_think = (data->tt_eat + data->tt_sleep) / 3;

    // // Add a small stagger based on the philosopher's ID to reduce contention
    // tt_think += (philo->id % 2) * 10;

    // // Ensure the philosopher doesn't think too long and risk starvation
    // if (tt_think > data->tt_die / 2)
	// 	tt_think = data->tt_die / 2;

    // Calculate the time remaining until the philosopher dies
    time_until_death = (philo->last_meal_time + (data->tt_die * 1000)) - get_time(MICROSECOND);
	tt_think = time_until_death / 2;
    // Ensure the think time is not negative or too short
    if (tt_think < 0)
		tt_think = 0;
	print_status(philo, THINKING);
	better_usleep(tt_think, data);
}


void	*simulation(void *value)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)value;
	data = philo->data;
	// while (!get_bool(&data->data_mutex, &data->philos_ready))
	// 	usleep(0);
	increment_long(&data->data_mutex, &data->n_running_philos);
	while (!all_philos_running(&data->data_mutex, &data->n_running_philos, data->nb_philos)) //mutex correcto?
		usleep(0) ;
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
	safe_mutex_handle(&philo->philo_mutex, (t_thrhandle){LOCK, __FILE__, __LINE__});
	elapsed = get_time(MICROSECOND) - philo->last_meal_time;;
	safe_mutex_handle(&philo->philo_mutex, (t_thrhandle){UNLOCK, __FILE__, __LINE__});
	if (elapsed > (philo->data->tt_die * 1000))
		return (true);
	return (false);
}

void	*death_monitor(void *value)
{
	t_data	*data;
	int		i;

	data = (t_data *)value;
	while (!all_philos_running(&data->data_mutex, &data->n_running_philos, data->nb_philos)) //mutex correcto?
		usleep(0) ;
	while (!get_bool(&data->data_mutex, &data->end_sim))
	{
		i = -1;
		while (++i < data->nb_philos)
		{
			if (philo_died(&data->philos[i]))
			{
				print_status(&data->philos[i], DIED);
				set_bool(&data->data_mutex, &data->end_sim, true);
				break;
			}
		}
		if (all_philos_are_full(data))
		{
			printf("\t\t\tAll philos are full\n");//delete this print
			set_bool(&data->data_mutex, &data->end_sim, true);
			break;
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
	set_long(&data->data_mutex, &data->start_sim, get_time(MICROSECOND));
	set_bool(&data->data_mutex, &data->philos_ready, true);
	safe_thread_handle(&data->monitor, death_monitor, data, (t_thrhandle){CREATE, __FILE__, __LINE__});
	i = -1;
	while (++i < data->nb_philos)
		safe_thread_handle(&data->philos[i].thread_id, NULL, NULL, (t_thrhandle){JOIN, __FILE__, __LINE__});
	set_bool(&data->data_mutex, &data->end_sim, true);
	safe_thread_handle(&data->monitor, NULL, NULL, (t_thrhandle){JOIN, __FILE__, __LINE__});
}
