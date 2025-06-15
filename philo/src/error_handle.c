/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomorale <lomorale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:26:24 by odruke-s          #+#    #+#             */
/*   Updated: 2025/04/26 10:25:24 by lomorale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

char	*mutex_error(int status)
{
	if (status == EINVAL)
		return ("The value at mutex or atrr is invalid");
	else if	(status == EDEADLK)
		return ("A deadlock would occur if the thread blocked waiting for mutex");
	else if (status == EPERM)
		return ("The current thread does not hold a lock on mutex");
	else if (status == ENOMEM)
		return ("The process cannot allocate enough memory to create another mutex");
	else if (status == EBUSY)
		return ("Mutex is locked");
	return (NULL);
}
char	*thread_error(int status, int code)
{
	if (status == EINVAL && code == CREATE)
		return ("Invalid settings in attr");
	else if(status == EINVAL && code == JOIN)
		return ("thread is not a joinable thread or another \
			thread is already waiting to join with this thread");
	else if (status == EAGAIN)
		return ("Insufficient resources to create another thread");
	else if (status == EPERM)
		return ("Atrr does not have appropiate permission");
	else if (status == ESRCH)
		return ("No thread with the ID thread could be found");
	else if	(status == EDEADLK)
		return ("A deadlock was detected ");
	return (NULL);
}

void	free_table(void **table)
{
	int	i;

	i = -1;
	while(table[++i])
		free(table[i]);
	free(table);
	table = NULL;
}
void	free_data(t_data *data)
{
	int i;

	if (data)
	{
		if (data->philos)
		{
			i = -1;
			while (++i < data->nb_philos)
				safe_mutex_handle(&data->philos[i].philo_mutex, (t_thrhandle){DESTROY, __FILE__, __LINE__});
			free(data->philos);
		}
		if (data->forks)
		{
			i = -1;
			while (++i < data->nb_philos)
				safe_mutex_handle(&data->forks[i].fork, (t_thrhandle){DESTROY, __FILE__, __LINE__});
			free(data->forks);
		}
		safe_mutex_handle(&data->data_mutex, (t_thrhandle){DESTROY, __FILE__, __LINE__});
		safe_mutex_handle(&data->print_mutex, (t_thrhandle){DESTROY, __FILE__, __LINE__});
		free(data);
	}
}

const t_errinfo	*get_errinfo(t_error error)
{
	static t_errinfo	errtab[ERR_UNKNOWN + 1];

	errtab[ERR_BAD_ARGS] = (t_errinfo){ERR_BAD_ARGS, EXIT_FAILURE,
		"Usage:\nNumber of philosophers | time to die | time to eat | \
time to sleep || Optional: nummber of times philosopher must eat\n"};
	errtab[ERR_NO_NUMBER] = (t_errinfo){ERR_NO_NUMBER, EXIT_FAILURE,
		"Invalid input: all arguments must be unsigned integers\n"};
	errtab[ERR_MALLOC] = (t_errinfo){ERR_MALLOC, EXIT_FAILURE,
		"Malloc failed at: %s:%i\n"};
	errtab[ERR_MAX_LONG] = (t_errinfo){ERR_MAX_LONG, EXIT_FAILURE,
		"%s = %s: Excede LONG_MAX. Choose a lower value\n"};
	errtab[ERR_MIN_TIME] = (t_errinfo){ERR_MIN_TIME, EXIT_FAILURE,
		"%s = %s: Is less than 60ms. Please test with a higher value\n"};
	errtab[ERR_MAX_PHILO] = (t_errinfo){ERR_MAX_PHILO, EXIT_FAILURE,
		"philosophers = %s: It should be between 1 and %s\n"};
	errtab[ERR_MUTEX] = (t_errinfo){ERR_MUTEX, EXIT_FAILURE,
		"Mutex: %s at %s:%i\n"};
	errtab[ERR_THREAD] = (t_errinfo){ERR_THREAD, EXIT_FAILURE,
		"Threads: %s at %s:%i\n"};
	errtab[ERR_VOID] = (t_errinfo){ERR_VOID, EXIT_FAILURE, ""};
	errtab[ERR_UNKNOWN] = (t_errinfo){ERR_UNKNOWN, 1, "%s\n"};
	return (&errtab[error]);
}

int	error_handle(t_error error, t_errarg err_args)
{
	const t_errinfo	*err_info;
	t_data			*data;

	data = recover_data_address(NULL);
	err_info = get_errinfo(error);
	if (!err_args.extra)
		printf(err_info->str_format, err_args.cmd, err_args.code);
	else
		printf(err_info->str_format, err_args.cmd, err_args.extra, err_args.code);
	if (err_args.terminate)
	{
		free_data(data);
		exit(err_info->exit_code);
	}
	else
		return (err_info->exit_code);
}
