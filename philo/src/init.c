/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                      +:+                   */
/*   By: odruke-s <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/06/16 13:25:20 by odruke-s       #+#    #+#                */
/*   Updated: 2025/06/18 21:29:07 by odruke-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	assign_forks(t_philo *philo, t_fork *forks, int idx, int nb_philos)
{
	philo->fork[0] = &forks[(idx +1) % nb_philos];
	philo->fork[1] = &forks[idx];
	if ((idx % 2) == 0)
	{
		philo->fork[0] = &forks[idx];
		philo->fork[1] = &forks[(idx +1) % nb_philos];
	}
}

static void	init_philo(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->nb_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].is_full = false;
		data->philos[i].meals_counter = 0;
		data->philos[i].data = data;
		data->philos[i].last_meal_time = 0;
		safe_mutex_handle(&data->philos[i].philo_mutex,
			(t_thrhandle){INIT, __FILE__, __LINE__});
		assign_forks(&data->philos[i], data->forks, i, data->nb_philos);
	}
}

void	fill_data(t_data *data)
{
	data->end_sim = false;
	data->forks = NULL;
	data->philos = NULL;
	data->limit_meals = -1;
	data->nb_philos = 0;
	data->start_sim = 0;
	data->n_running_philos = 0;
	data->philos_ready = false;
	data->tt_die = 0;
	data->tt_eat = 0;
	data->tt_sleep = 0;
}

void	init_data(t_data *data)
{
	int	i;

	i = -1;
	data->end_sim = false;
	data->philos = (t_philo *)safe_malloc(sizeof(t_philo) * data->nb_philos,
			__FILE__, __LINE__);
	data->forks = (t_fork *)safe_malloc(sizeof(t_fork) * data->nb_philos,
			__FILE__, __LINE__);
	safe_mutex_handle(&data->data_mutex, (t_thrhandle){INIT,
		__FILE__, __LINE__});
	safe_mutex_handle(&data->print_mutex, (t_thrhandle){INIT,
		__FILE__, __LINE__});
	while (++i < data->nb_philos)
	{
		safe_mutex_handle(&data->forks[i].fork,
			(t_thrhandle){INIT, __FILE__, __LINE__});
		data->forks[i].fork_id = i;
	}
	init_philo(data);
}
