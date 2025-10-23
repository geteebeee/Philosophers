/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gorg <gorg@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 22:28:29 by gorg              #+#    #+#             */
/*   Updated: 2025/05/16 18:18:23 by gorg             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// Basic version of atoi with overflow protection.
int	ft_atoi(const char *str)
{
	int		res;
	int		s;
	char	*p;

	s = 1;
	res = 0;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			s *= -1;
		str++;
	}
	while (*str == '0')
		str++;
	p = (char *)str;
	while (*str >= '0' && *str <= '9')
		res = res * 10 + (*str++ - '0');
	if (str - p >= 20 && s == 1)
		return (-1);
	else if (str - p >= 20 && s == -1)
		return (0);
	return (s * res);
}

//Still atoi mod
static int	atoirun(const char *str, long long res, int sign, int *error_flag)
{
	while (*str >= '0' && *str <= '9')
	{
		res = res * 10 + (*str - '0');
		if ((sign == 1 && res > INT_MAX) || (sign == -1 && - res < INT_MIN)
			|| res == 0)
		{
			*error_flag = 1;
			return (0);
		}
		str++;
	}
	return ((int)(res * sign));
}

//Safer atoi with error flagging for invalid input
int	ft_atoi_mod(const char *str, int *error_flag)
{
	long long	res;
	int			sign;

	res = 0;
	sign = 1;
	if (*error_flag == 1)
		return (0);
	while ((*str >= 9 && *str <= 13) || *str == ' ')
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	if (*str < '0' || *str > '9')
	{
		*error_flag = 1;
		return (0);
	}
	return (atoirun(str, res, sign, error_flag));
}

//Argument validator
int	ft_check(char **str, char *charset)
{
	char	*current_char;
	char	*valid_char;
	int		err;

	err = 0;
	while (*(++str))
	{
		current_char = *str;
		ft_atoi_mod(current_char, &err);
		while (*current_char)
		{
			valid_char = charset;
			while (*valid_char)
			{
				if (*current_char == *valid_char)
					break ;
				valid_char++;
			}
			if (!*valid_char || err == 1)
				return (-1);
			current_char++;
		}
	}
	return (0);
}

// Joins philosopher and monitor threads, destroys mutexes and monitor
int	joinerofdestroyers(t_data *data, int n)
{
	int	i;

	i = 0;
	while (i < data->philos)
	{
		if (pthread_join(data->id[i].thread_id, NULL) != 0)
			printf("This guy thread you know %d joinen't\n", i);
		i++;
	}
	if (n == 1)
	{
		if (pthread_join(data->monitor, NULL) != 0)
			printf("Monitor is not joinering\n");
	}
	i = 0;
	while (i < data->philos)
	{
		if (pthread_mutex_destroy(&data->forks[i]) != 0)
			printf("Destruction failed\n");
		i++;
	}
	pthread_mutex_destroy(&data->d_lock);
	pthread_mutex_destroy(&data->m_lock);
	return (0);
}
