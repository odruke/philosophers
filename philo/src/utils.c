/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                      +:+                   */
/*   By: odruke-s <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/06/16 13:56:04 by odruke-s       #+#    #+#                */
/*   Updated: 2025/06/18 22:30:53 by odruke-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time(t_timecode code)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		error_handle(ERR_UNKNOWN,
			(t_errarg){"gettimeofday failed", NULL, 0, KILL});
	if (code == SECOND)
		return (tv.tv_sec);
	else if (code == MILISECOND)
		return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
	else if (code == MICROSECOND)
		return ((tv.tv_sec * 1000000) + tv.tv_usec);
	else
		error_handle(ERR_UNKNOWN,
			(t_errarg){"bad input on get_time", NULL, 0, KILL});
	return (ERR_UNKNOWN);
}

void	better_usleep(long microsec, t_data *data)
{
	long	start;
	long	elapsed;
	long	remaining;

	start = get_time(MICROSECOND);
	while ((get_time(MICROSECOND) - start) < microsec)
	{
		if (get_bool(&data->data_mutex, &data->end_sim))
			break ;
		elapsed = (get_time(MICROSECOND) - start);
		remaining = microsec - elapsed;
		if (remaining > 1000)
			usleep(remaining / 2);
		else
		{
			while ((get_time(MICROSECOND) - start) < microsec)
				;
		}
	}
}

bool	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (true);
	else
		return (false);
}

bool	is_all_nb(int ac, char **av)
{
	int	i;
	int	b;

	i = 1;
	while (i < ac)
	{
		b = 0;
		while (av[i][b])
		{
			if (av[i][b] < '0' || av[i][b] > '9')
				return (false);
			b++;
		}
		i++;
	}
	return (true);
}

long	ft_atol(const char *nb)
{
	long long	res;
	int			i;

	res = 0;
	i = 0;
	if (nb[0] == '0')
		return (0);
	while (nb[i] == 32 || (nb[i] >= 9 && nb[i] <= 13))
		i++;
	while (ft_isdigit(nb[i]))
	{
		res += nb[i] - 48;
		if (ft_isdigit(nb[i + 1]))
			res *= 10;
		i++;
	}
	if (res > LONG_MAX)
		return (-1);
	return ((long)res);
}
