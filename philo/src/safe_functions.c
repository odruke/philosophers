#include "philo.h"

void	*safe_malloc(size_t bytes, char *file, int line)
{
	void	*res;

	res = malloc(bytes);
	if (!res)
		error_handle(ERR_MALLOC, (t_errarg){file, NULL, line, KILL});
	return (res);
}

void	safe_mutex_handle(pthread_mutex_t *mutex, t_thrhandle codes)
{
	char	*errmsg;

	errmsg = NULL;
	if (INIT == codes.pthcode)
	{
		// printf("created mutex:%p \n", (void *)mutex);
		errmsg =  mutex_error(pthread_mutex_init(mutex, NULL));
	}

	else if (LOCK == codes.pthcode)
	{
		errmsg =  mutex_error(pthread_mutex_lock(mutex));
		// printf("mutex: %p lock\n", (void *)mutex);
		// printf("mutex is locked\n");
	}
	else if (UNLOCK == codes.pthcode)
	{
		errmsg =  mutex_error(pthread_mutex_unlock(mutex));
		// printf("mutex: %p unlock\n", (void *)mutex);
		// printf("mutex is unlocked\n");
	}
	else if (DESTROY == codes.pthcode)
	{
		// printf("destroying mutex:%p \n", (void *)mutex);
		errmsg =  mutex_error(pthread_mutex_destroy(mutex));
	}
	else
		error_handle(ERR_MUTEX, (t_errarg){"Bad t_pthcode code", codes.file, codes.line, KILL});
	if (errmsg)
		error_handle(ERR_MUTEX, (t_errarg){errmsg, codes.file, codes.line, KILL});

}

void safe_thread_handle(pthread_t *thread, void *(*f)(void *), void *data, t_thrhandle codes)
{
	char	*errmsg;

	errmsg = NULL;
	if (codes.pthcode == CREATE)
	{
		errmsg = thread_error(pthread_create(thread, NULL, f, data), codes.pthcode);
		// printf("created thread:%p\n", (void *)thread);
	}

	else if (codes.pthcode == JOIN)
	{
		errmsg = thread_error(pthread_join(*thread, NULL), codes.pthcode);//revisar
		// printf("join thread:%p\n", (void *)thread);
	}
	else
		error_handle(ERR_THREAD, (t_errarg){"Bad t_pthcode code", codes.file, codes.line, KILL});
	if (errmsg)
		error_handle(ERR_THREAD, (t_errarg){errmsg, codes.file, codes.line, KILL});
}
