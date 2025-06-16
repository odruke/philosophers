/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   print_functions.c                                   :+:    :+:           */
/*                                                      +:+                   */
/*   By: odruke-s <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/06/16 13:41:40 by odruke-s       #+#    #+#                */
/*   Updated: 2025/06/16 13:47:49 by odruke-s       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
/*
void	mutex_is_locked(pthread_mutex_t *mutex, t_philo *philo,
	char *file, int line)
{
	if (pthread_mutex_trylock(mutex) == EBUSY)
	{
    	fprintf(stderr, ">>> philo[%d] mutex still locked at %s:%i!\n",
			philo->id, file, line);
		return ;
	}
	pthread_mutex_unlock(mutex);
}

void	print_status(t_philo *philo, t_philo_status status)
{
	long	elapsed_time;
	t_data	*data;

	data = philo->data;
	elapsed_time = (get_time(MILISECOND) - data->start_sim);
	//check if philo is full?
	safe_mutex_handle(&data->print_mutex,
		(t_thrhandle){LOCK, __FILE__, __LINE__});
	if (status == TAKE_FORK && !get_bool(&data->data_mutex, &data->end_sim))
		printf("%-6ld %i has taken a fork.\t\tfirst fork_id = %i\t\tsecond \
			fork_id = %i\n", elapsed_time, philo->id,
				philo->fork[0]->fork_id, philo->fork[1]->fork_id);
	else if (status == LEAVE_FORK && !get_bool(&data->data_mutex,
		&data->end_sim))
		printf("%-6ld %i has left a fork.\t\tfirst fork_id = %i\t\tsecond \
			fork_id = %i\n", elapsed_time, philo->id, philo->fork[0]->fork_id,
				philo->fork[1]->fork_id);
	else if (status == EATING && !get_bool(&data->data_mutex, &data->end_sim))
		printf("%-6ld %i is eating\n", elapsed_time, philo->id);
	else if (status == SLEEPING && !get_bool(&data->data_mutex, &data->end_sim))
		printf("%-6ld %i is sleeping\n", elapsed_time, philo->id);
	else if (status == THINKING && !get_bool(&data->data_mutex, &data->end_sim))
		printf("%-6ld %i is thinking\n", elapsed_time, philo->id);
	else if (status == DIED && !get_bool(&data->data_mutex, &data->end_sim))
		printf("%-6ld %i died\n", elapsed_time, philo->id);
	safe_mutex_handle(&data->print_mutex,
		(t_thrhandle){UNLOCK, __FILE__, __LINE__});
}
*/
void	print_status(t_philo *philo, t_philo_status status)
{
	long	elapsed_time;
	t_data	*data;

	data = philo->data;
	if (philo->is_full || get_bool(&data->data_mutex, &data->end_sim))
		return ;
	elapsed_time = (get_time(MICROSECOND) - data->start_sim);//try get_time in milisec and start sim /1000
	elapsed_time /= 1000;
	safe_mutex_handle(&data->print_mutex,
		(t_thrhandle){LOCK, __FILE__, __LINE__});
	if (status == TAKE_FORK)
		printf("%-6ld %i has taken a fork\n", elapsed_time, philo->id);
	else if (status == EATING)
		printf("%-6ld %i is eating\n", elapsed_time, philo->id);
	else if (status == SLEEPING)
		printf("%-6ld %i is sleeping\n", elapsed_time, philo->id);
	else if (status == THINKING)
		printf("%-6ld %i is thinking\n", elapsed_time, philo->id);
	else if (status == DIED)
		printf("%-6ld %i died\n", elapsed_time, philo->id);
	safe_mutex_handle(&data->print_mutex,
		(t_thrhandle){UNLOCK, __FILE__, __LINE__});
}
