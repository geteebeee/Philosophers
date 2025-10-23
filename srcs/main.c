/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gorg <gorg@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 15:45:02 by gorg              #+#    #+#             */
/*   Updated: 2025/05/16 18:59:13 by gorg             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//Checks if a specific philosopher has died and sets the ded flag
static int	monitor_loop(t_data *data, int index)
{
	int	elapsed;
	int	ded;

	pthread_mutex_lock(&data->d_lock);
	pthread_mutex_lock(&data->m_lock);
	if (data->max_cycles > 0 && data->id[index].cycles >= data->max_cycles)
	{
		unlocker(data);
		return (0);
	}
	ded = data->ded;
	unlocker(data);
	if (!ded)
	{
		elapsed = time_diff(data, &data->id[index], 2, 0);
		if (elapsed > data->id[index].t2d)
		{
			pthread_mutex_lock(&data->d_lock);
			data->ded = 1;
			pthread_mutex_unlock(&data->d_lock);
			stamp(data, &data->id[index], 5);
			return (1);
		}
	}
	return (0);
}

//Iterates through all philosophers to check if any have died.
static int	check_death(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philos)
	{
		if (monitor_loop(data, i))
			return (1);
		i++;
	}
	return (0);
}

//Verifies if all philosophers have completed the required eating cycles
static int	check_cycles(t_data *data)
{
	int	i;

	i = 0;
	if (data->max_cycles <= 0)
		return (0);
	while (i < data->philos)
	{
		pthread_mutex_lock(&data->m_lock);
		if (data->id[i].cycles < data->max_cycles)
		{
			pthread_mutex_unlock(&data->m_lock);
			return (0);
		}
		pthread_mutex_unlock(&data->m_lock);
		i++;
	}
	return (1);
}

//Monitor thread function that terminates the simulation when needed
static void	*monitor_philos(void *arg)
{
	t_data	*data;
	int		soon;

	data = (t_data *)arg;
	while (!dead_check(data) && !(check_death(data) || check_cycles(data)))
	{
		soon = next_deadline_ms(data);
		if (soon == INT_MAX)
			usleep(5000);
		else if (soon > 2)
			usleep((soon - 1) * 1000);
		else
			usleep(500);
	}
	pthread_mutex_lock(&data->d_lock);
	data->ded = 1;
	pthread_mutex_unlock(&data->d_lock);
	return (NULL);
}

//THEMAIN! parses input, sets up data, starts threads
int	main(int argc, char **argv)
{
	t_data	data;

	if ((argc != 6 && argc != 5) || ft_check(argv, "1234567890") == -1)
	{
		printf("./philo <philos> <time to die> <time to eat> <time to sleep> ");
		printf("(optional) <number of cycles>\n*Allowed values 1 - INT_MAX\n");
		return (1);
	}
	if (list_data(&data, argv, argc) == -1)
	{
		printf("Error: Memory allocation failed.\n");
		return (1);
	}
	if (pthread_create(&data.monitor, NULL, monitor_philos, &data) != 0)
	{
		joinerofdestroyers(&data, 0);
		free(data.id);
		free(data.forks);
		return (1);
	}
	joinerofdestroyers(&data, 1);
	free(data.id);
	free(data.forks);
	return (0);
}
