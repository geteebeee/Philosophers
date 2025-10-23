/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gorg <gorg@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 22:44:02 by gorg              #+#    #+#             */
/*   Updated: 2025/05/09 22:28:49 by gorg             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//Safe dead check
int	dead_check(t_data *data)
{
	int	dead;

	pthread_mutex_lock(&data->d_lock);
	dead = data->ded;
	pthread_mutex_unlock(&data->d_lock);
	return (dead);
}

//Sets delay for simulation start
void	set_timer(t_data *data)
{
	gettimeofday(&data->b_time, NULL);
	data->b_time.tv_sec += 2;
}

//As the name states
void	release_forks(t_id *philo)
{
	pthread_mutex_unlock(philo->l_lock);
	pthread_mutex_unlock(philo->r_lock);
}

void	ft_putchar_fd(char s, int fd)
{
	ssize_t	bytes_written;
	ssize_t	ret;

	bytes_written = write(fd, &s, 1);
	if (bytes_written == -1 || (size_t)bytes_written < 1)
	{
		ret = write(2, "Write error\n", 12);
		(void)ret;
	}
}

int	safe_gettimeofday(struct timeval *tv, t_data *data)
{
	if (gettimeofday(tv, NULL) == -1)
	{
		ft_putstr_fd("gettimeofday error\n", 1);
		pthread_mutex_lock(&data->d_lock);
		data->ded = 1;
		pthread_mutex_unlock(&data->d_lock);
		return (-1);
	}
	return (0);
}
