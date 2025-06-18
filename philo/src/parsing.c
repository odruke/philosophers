/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   parsing.c                                           :+:    :+:           */
/*                                                      +:+                   */
/*   By: odruke-s <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/06/16 13:32:28 by odruke-s       #+#    #+#                */
/*   Updated: 2025/06/16 13:40:40 by odruke-s       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	check_min_time_to(t_data *data, char **av)
{
	bool	state;

	state = true;
	if (data->tt_die < 60)
	{
		error_handle(ERR_MIN_TIME,
			(t_errarg){"time to die", av[2], 0, CONTINUE});
		state = false;
	}
	if (data->tt_eat < 60)
	{
		error_handle(ERR_MIN_TIME,
			(t_errarg){"time to eat", av[3], 0, CONTINUE});
		state = false;
	}
	if (data->tt_sleep < 60)
	{
		error_handle(ERR_MIN_TIME,
			(t_errarg){"time to sleep", av[4], 0, CONTINUE});
		state = false;
	}
	if (!state)
		error_handle(ERR_VOID, (t_errarg){"", "", 0, KILL});
}

void	parsing(t_data *data, char **av)
{
	data->nb_philos = ft_atol(av[1]);
	if (data->nb_philos < 0)
		error_handle(ERR_MAX_LONG,
			(t_errarg){"number of philos", av[1], 0, KILL});
	else if (data->nb_philos > MAX_PHILOS || !data->nb_philos)
		error_handle(ERR_MAX_PHILO, (t_errarg){av[1], MAX_PHILOS_STR, 0, KILL});
	data->tt_die = ft_atol(av[2]);
	if (data->tt_die < 0)
		error_handle(ERR_MAX_LONG, (t_errarg){"time to die", av[2], 0, KILL});
	data->tt_eat = ft_atol(av[3]);
	if (data->tt_eat < 0)
		error_handle(ERR_MAX_LONG, (t_errarg){"time to eat", av[3], 0, KILL});
	data->tt_sleep = ft_atol(av[4]);
	if (data->tt_sleep < 0)
		error_handle(ERR_MAX_LONG, (t_errarg){"time to sleep", av[4], 0, KILL});
	if (av[5])
	{
		data->limit_meals = ft_atol(av[5]);
		if (data->limit_meals < 0)
			error_handle(ERR_MAX_LONG,
				(t_errarg){"meal limit", av[5], 0, KILL});
	}
	else
		data->limit_meals = -1;
	check_min_time_to(data, av);
}

void	input_validation(int ac, char **av)
{
	if (ac < 5 || ac > 6)
		error_handle(ERR_BAD_ARGS, (t_errarg){NULL, NULL, 0, KILL});
	if (!is_all_nb(ac, av))
		error_handle(ERR_NO_NUMBER, (t_errarg){NULL, NULL, 0, KILL});
	(void)av;
}
