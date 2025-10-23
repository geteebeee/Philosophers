/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gorg <gorg@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 16:46:41 by gorg              #+#    #+#             */
/*   Updated: 2025/05/16 18:57:08 by gorg             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//Handles the case where a philosopher has only one fork
void	*solomen(t_id *id, t_data *data)
{
	stamp(data, id, 1);
	usleep(id->t2d * 1000);
	return (NULL);
}

// Cleans up resources if thread or mutex creation fails.
static int	another_cleaner(t_data *data, int i, int op)
{
	data->ded = 1;
	if (op == 1)
	{
		if (pthread_mutex_destroy(&data->forks[i]) != 0)
			printf("Destruction failed\n");
	}
	data->philos = i;
	joinerofdestroyers(data, 0);
	free(data->id);
	free(data->forks);
	return (-1);
}

// Initializes values for each philosopher and starts their threads.
static int	data_values(t_data *data, char **argv)
{
	int	i;

	i = 0;
	while (i < data->philos)
	{
		data->id[i].id = i + 1;
		data->id[i].cycles = 0;
		data->id[i].t2d = ft_atoi(argv[2]);
		data->id[i].t2e = ft_atoi(argv[3]);
		data->id[i].t2s = ft_atoi(argv[4]);
		data->id[i].data = data;
		data->id[i].last_meal = data->b_time;
		data->id[i].food_delay = 0;
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			return (another_cleaner(data, i, 0));
		data->id[i].l_lock = &data->forks[i];
		data->id[i].r_lock = &data->forks[(i + 1) % data->philos];
		if (pthread_create(&data->id[i].thread_id, NULL,
				run_cycle, &data->id[i]) != 0)
			return (another_cleaner(data, i, 1));
		i++;
	}
	return (0);
}

//Initializes the mutexes
static int	init_mutexes(t_data *data)
{
	if (pthread_mutex_init(&data->d_lock, NULL) != 0)
	{
		free(data->id);
		free(data->forks);
		return (-1);
	}
	if (pthread_mutex_init(&data->m_lock, NULL) != 0)
	{
		free(data->id);
		free(data->forks);
		pthread_mutex_destroy(&data->d_lock);
		return (-1);
	}
	return (0);
}

//Sets up the simulation data
int	list_data(t_data *data, char **argv, int argc)
{
	data->philos = ft_atoi(argv[1]);
	if (data->philos <= 0)
		return (-1);
	data->id = malloc(sizeof(t_id) * data->philos);
	if (!data->id)
		return (-1);
	memset(data->id, 0, sizeof(t_id) * data->philos);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->philos);
	if (!data->forks)
	{
		free(data->id);
		return (-1);
	}
	if (argc == 6)
		data->max_cycles = ft_atoi(argv[5]);
	else
		data->max_cycles = 0;
	data->ded = 0;
	if (init_mutexes(data) == -1)
		return (-1);
	set_timer(data);
	if (data_values(data, argv) == -1)
		return (-1);
	return (0);
}
