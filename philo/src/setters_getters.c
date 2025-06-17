/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   setters_getters.c                                   :+:    :+:           */
/*                                                      +:+                   */
/*   By: odruke-s <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/06/16 13:52:13 by odruke-s       #+#    #+#                */
/*   Updated: 2025/06/16 13:52:19 by odruke-s       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_bool(pthread_mutex_t *mutex, bool *dest, bool value)
{
	safe_mutex_handle(mutex, (t_thrhandle){LOCK, __FILE__, __LINE__});
	*dest = value;
	safe_mutex_handle(mutex, (t_thrhandle){UNLOCK, __FILE__, __LINE__});
}

bool	get_bool(pthread_mutex_t *mutex, bool *value)
{
	bool	res;

	safe_mutex_handle(mutex, (t_thrhandle){LOCK, __FILE__, __LINE__});
	res = *value;
	safe_mutex_handle(mutex, (t_thrhandle){UNLOCK, __FILE__, __LINE__});
	return (res);
}

void	set_long(pthread_mutex_t *mutex, long *dest, long value)
{
	safe_mutex_handle(mutex, (t_thrhandle){LOCK, __FILE__, __LINE__});
	*dest = value;
	safe_mutex_handle(mutex, (t_thrhandle){UNLOCK, __FILE__, __LINE__});
}

long	get_long(pthread_mutex_t *mutex, long *value)
{
	long	res;

	safe_mutex_handle(mutex, (t_thrhandle){LOCK, __FILE__, __LINE__});
	res = *value;
	safe_mutex_handle(mutex, (t_thrhandle){UNLOCK, __FILE__, __LINE__});
	return (res);
}

void	increment_long(pthread_mutex_t *mutex, long *value)
{
	safe_mutex_handle(mutex, (t_thrhandle){LOCK, __FILE__, __LINE__});
	*value = *value + 1;
	safe_mutex_handle(mutex, (t_thrhandle){UNLOCK, __FILE__, __LINE__});
}
