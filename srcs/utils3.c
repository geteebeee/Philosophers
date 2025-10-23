/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gorg <gorg@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 22:52:44 by gorg              #+#    #+#             */
/*   Updated: 2025/05/16 18:59:22 by gorg             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	unlocker(t_data *data)
{
	pthread_mutex_unlock(&data->m_lock);
	pthread_mutex_unlock(&data->d_lock);
}

void	ft_putstr_fd(char *s, int fd)
{
	size_t	len;
	char	*ptr;
	ssize_t	bytes_written;
	ssize_t	ret;

	len = 0;
	ptr = s;
	if (s)
	{
		while (*ptr != '\0')
		{
			len++;
			ptr++;
		}
		bytes_written = write(fd, s, len);
		if (bytes_written == -1 || (size_t)bytes_written < len)
		{
			ret = write(2, "Write error\n", 12);
			(void)ret;
		}
	}
}

// Returns the shortest time-to-die still pending,
//INT_MAX means no one can die right now (all full / finished / already dead)
int	next_deadline_ms(t_data *data)
{
	int	i;
	int	soon;
	int	left;

	i = 0;
	soon = INT_MAX;
	while (i < data->philos)
	{
		if (data->max_cycles > 0 && data->id[i].cycles >= data->max_cycles)
		{
			i++;
			continue ;
		}
		left = data->id[i].t2d - time_diff(data, &data->id[i], 2, 0);
		if (left < soon)
			soon = left;
		i++;
	}
	return (soon);
}
