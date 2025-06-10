
#include "philo.h"

void	print_status(t_philo *philo, t_philo_status status)
{
	long	elapsed_time;
	t_data	*data;

	data = philo->data;
	elapsed_time = (get_time(MILISECOND) - data->start_sim);
	//check if philo is full?
	safe_mutex_handle(&data->print_mutex, (t_thrhandle){LOCK, __FILE__, __LINE__});
	if (status == TAKE_FORK && !get_bool(&data->data_mutex, data->end_sim))
		printf("%-6ld %i has taken a fork\n", elapsed_time, philo->id);
	else if (status == EATING && !get_bool(&data->data_mutex, data->end_sim))
		printf("%-6ld %i is eating\n", elapsed_time, philo->id);
	else if (status == SLEEPING && !get_bool(&data->data_mutex, data->end_sim))
		printf("%-6ld %i is sleeping\n", elapsed_time, philo->id);
	else if (status == THINKING && !get_bool(&data->data_mutex, data->end_sim))
		printf("%-6ld %i is thinking\n", elapsed_time, philo->id);
	else if (status == DIED && !get_bool(&data->data_mutex, data->end_sim))
		printf("%-6ld %i died\n", elapsed_time, philo->id);
	safe_mutex_handle(&data->print_mutex, (t_thrhandle){UNLOCK, __FILE__, __LINE__});
}
