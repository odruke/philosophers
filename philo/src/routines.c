/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odruke-s <odruke-s@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 22:33:10 by odruke-s          #+#    #+#             */
/*   Updated: 2025/06/18 22:33:16 by odruke-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eat_routine(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	safe_mutex_handle(&philo->fork[0]->fork,
		(t_thrhandle){LOCK, __FILE__, __LINE__});
	print_status(philo, TAKE_FORK);
	safe_mutex_handle(&philo->fork[1]->fork,
		(t_thrhandle){LOCK, __FILE__, __LINE__});
	print_status(philo, TAKE_FORK);
	set_long(&philo->philo_mutex, &philo->last_meal_time,
		get_time(MICROSECOND));
	philo->meals_counter++;
	print_status(philo, EATING);
	better_usleep(data->tt_eat * 1000, data);
	safe_mutex_handle(&philo->fork[0]->fork,
		(t_thrhandle){UNLOCK, __FILE__, __LINE__});
	print_status(philo, LEAVE_FORK);
	safe_mutex_handle(&philo->fork[1]->fork,
		(t_thrhandle){UNLOCK, __FILE__, __LINE__});
	print_status(philo, LEAVE_FORK);
	if (data->limit_meals > 0 && philo->meals_counter == data->limit_meals)
		set_bool(&philo->philo_mutex, &philo->is_full, true);
}

void	sleep_routine(t_philo *philo)
{
	print_status(philo, SLEEPING);
	better_usleep(philo->data->tt_sleep * 1000, philo->data);
}

void	think_routine(t_philo *philo)
{
	t_data	*data;
	long	tt_think;
	long	t_sleep;
	long	t_eat;

	data = philo->data;
	t_sleep = data->tt_sleep * 1000;
	t_eat = data->tt_eat * 1000;
	tt_think = 0;
	if (!(data->nb_philos % 2))
		return (print_status(philo, THINKING), better_usleep(tt_think, data));
	tt_think = t_eat * 2 - t_sleep;
	if (tt_think < 0)
		tt_think = 0;
	print_status(philo, THINKING);
	better_usleep(tt_think * 0.60, data);
}
