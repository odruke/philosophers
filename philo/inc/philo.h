
#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <limits.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <stdbool.h>
# include <errno.h>
# define KILL 1
# define CONTINUE 0
# define MAX_PHILOS 200
# define MAX_PHILOS_STR "200"

typedef struct s_data t_data;
typedef enum e_error
{
	ERR_BAD_ARGS,
    ERR_NO_NUMBER,
    ERR_MALLOC,
    ERR_MAX_LONG,
    ERR_MIN_TIME,
    ERR_MAX_PHILO,
    ERR_MUTEX,
    ERR_THREAD,
    ERR_VOID,
    ERR_UNKNOWN
}t_error;

typedef enum e_timecode
{
    SECOND,
    MILISECOND,
    MICROSECOND
}t_timecode;

typedef enum e_pthcode
{
    INIT,
    LOCK,
    UNLOCK,
    CREATE,
    DESTROY,
    JOIN,
    DETACH
}t_pthcode;

typedef enum e_philo_status
{
    EATING,
    SLEEPING,
    THINKING,
    TAKE_FORK,
    DIED
}t_philo_status;

typedef struct s_thrhandle
{
    t_pthcode   pthcode;
    char        *file;
    int         line;

}t_thrhandle;

typedef struct s_errarg
{
    char    *cmd;
    char    *extra;
    int     code;
    int     terminate;
}t_errarg;

typedef struct s_errinfo
{
	t_error		error;
	int			exit_code;
	const char	*str_format;
}t_errinfo;

typedef struct s_fork
{
    pthread_mutex_t fork;
    int             fork_id;
}t_fork;

typedef struct s_philo
{
    int             id;
    int             meals_counter;
    bool            is_full;
    long            last_meal_time;
    t_fork          fork[2];
    pthread_t       thread_id;
    pthread_mutex_t philo_mutex;
    t_data          *data;
}t_philo;

struct s_data
{
    long             nb_philos;
    long             tt_die;
    long             tt_eat;
    long             tt_sleep;
    long             limit_meals;
    long            start_sim;
    bool            end_sim;
    bool            philos_ready;
    pthread_mutex_t data_mutex;
    pthread_mutex_t print_mutex;
    t_fork          *forks;
    t_philo         *philos;
};

t_data	*recover_data_address(t_data *data);
void    fill_data(t_data *data);
void    init_data(t_data *data);
void	free_data(t_data *data);
int     error_handle(t_error error, t_errarg err_args);
char	*mutex_error(int status);
char	*thread_error(int status, int code);
void    input_validation(int ac, char **av);
void    parsing(t_data *data, char **av);
long     ft_atol(const char *nb);
bool    is_all_nb(int ac, char **av);
void	start_simulation(t_data *data);
void	*safe_malloc(size_t bytes, char *file, int line);
void    safe_thread_handle(pthread_t *thread, void *(*f)(void *), void *data, t_thrhandle codes);
void	safe_mutex_handle(pthread_mutex_t *mutex, t_thrhandle codes);
bool	get_bool(pthread_mutex_t *mutex, bool value);
long	get_long(pthread_mutex_t *mutex, long value);
void	set_long(pthread_mutex_t *mutex, long *dest, long value);
void	set_bool(pthread_mutex_t *mutex, bool *dest, bool value);
long	get_time(t_timecode code);
void	better_usleep(long microsec, t_data *data);
void	print_status(t_philo *philo, t_philo_status status);

#endif
