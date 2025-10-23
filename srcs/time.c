/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gorg <gorg@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 22:27:50 by gorg              #+#    #+#             */
/*   Updated: 2025/05/14 22:37:11 by gorg             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//Returns the elapsed time in milliseconds.
//If `n == 1`, compares with program start time.
//If `n == 2`, compares with philosopher's last meal.

static int	timer(int n, t_id *philo, long *sec, long *usec)
{
	struct timeval	tv;

	if (safe_gettimeofday(&tv, philo->data) == -1)
		return (-1);
	if (n == 1)
	{
		*sec = tv.tv_sec - philo->data->b_time.tv_sec;
		*usec = tv.tv_usec - philo->data->b_time.tv_usec;
	}
	else
	{
		pthread_mutex_lock(&philo->data->m_lock);
		*sec = tv.tv_sec - philo->last_meal.tv_sec;
		*usec = tv.tv_usec - philo->last_meal.tv_usec;
		pthread_mutex_unlock(&philo->data->m_lock);
	}
	return (0);
}

int	time_diff(t_data *data, t_id *philo, int n, int d)
{
	long			sec;
	long			usec;
	long			total_msec;
	int				delay;

	(void)data;
	delay = 0;
	if (timer(n, philo, &sec, &usec) == -1)
		return (-1);
	if (usec < 0)
	{
		sec -= 1;
		usec += 1000000L;
	}
	if (d == 1)
		delay = 999;
	total_msec = sec * 1000 + (usec + delay) / 1000;
	return ((int)total_msec);
}

/* fast int→string to STDOUT_FILENO */
void	ft_putnbr_fd(int n, int fd)
{
	int	digit;

	if (n == -2147483648)
		ft_putstr_fd("-2147483648", fd);
	else if (n < 0)
	{
		ft_putstr_fd("-", fd);
		n = -n;
		ft_putnbr_fd(n, fd);
	}
	else
	{
		if (n > 9)
		{
			ft_putnbr_fd(n / 10, fd);
			ft_putnbr_fd(n % 10, fd);
		}
		else
		{
			digit = n + 48;
			ft_putchar_fd(digit, fd);
		}
	}
}

void	stamp(t_data *data, t_id *philo, int option)
{
	int	time;

	pthread_mutex_lock(&data->d_lock);
	if (data->ded && option != 5)
	{
		pthread_mutex_unlock(&data->d_lock);
		return ;
	}
	time = time_diff(data, philo, 1, 1);
	if (option == 1)
		printf("%d %d has taken a fork\n", time, philo->id);
	else if (option == 2)
		printf("%d %d is eating\n", time, philo->id);
	else if (option == 3)
		printf("%d %d is sleeping\n", time, philo->id);
	else if (option == 4)
		printf("%d %d is thinking\n", time, philo->id);
	else if (option == 5)
		printf("%d %d died\n", time, philo->id);
	pthread_mutex_unlock(&data->d_lock);
}

//Sleeps for a specific time
void	precise_sleep(long long ms, t_data *data)
{
	struct timeval	start;
	struct timeval	current;
	long long		elapsed;

	if (safe_gettimeofday(&start, data) == -1)
		return ;
	if (ms > 2000)
		usleep(ms - 1000);
	while (1)
	{
		if (safe_gettimeofday(&current, data) == -1)
			return ;
		elapsed = (current.tv_sec - start.tv_sec) * 1000000LL
			+ (current.tv_usec - start.tv_usec);
		if (elapsed >= ms || dead_check(data))
			break ;
		usleep(100);
	}
}
