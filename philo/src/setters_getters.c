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
