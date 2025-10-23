/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cycle.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gorg <gorg@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 16:45:02 by gorg              #+#    #+#             */
/*   Updated: 2025/05/16 18:56:35 by gorg             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//Attempts to acquire both forks for a philosopher, in the correct order.
static int	grab_forks(t_id *philo, t_data *data)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	if (philo->id % 2 == 0)
	{
		first_fork = philo->r_lock;
		second_fork = philo->l_lock;
	}
	else
	{
		first_fork = philo->l_lock;
		second_fork = philo->r_lock;
	}
	if (pthread_mutex_lock(first_fork) != 0)
		return (1);
	stamp(data, philo, 1);
	if (pthread_mutex_lock(second_fork) != 0)
	{
		pthread_mutex_unlock(first_fork);
		return (1);
	}
	stamp(data, philo, 1);
	return (0);
}

//Executes one eating cycle for the philosopher
static void	eat_cycle(t_id *philo, t_data *data)
{
	long	elapsed;

	if (dead_check(data) || grab_forks(philo, data) != 0)
		return ;
	pthread_mutex_lock(&data->m_lock);
	if (safe_gettimeofday(&philo->last_meal, data) == -1)
	{
		pthread_mutex_unlock(&data->m_lock);
		release_forks(philo);
		return ;
	}
	pthread_mutex_unlock(&data->m_lock);
	elapsed = time_diff(data, philo, 1, 0);
	philo->food_delay = elapsed + philo->t2e + philo->t2s + 3;
	stamp(data, philo, 2);
	precise_sleep(philo->t2e * 1000LL, data);
	pthread_mutex_lock(&data->m_lock);
	philo->cycles++;
	pthread_mutex_unlock(&data->m_lock);
	release_forks(philo);
}

//As the name states
static void	wait_for_start(t_data *data)
{
	struct timeval	tv;
	long			time_left;

	while (1)
	{
		if (safe_gettimeofday(&tv, data) == -1)
			return ;
		time_left = (data->b_time.tv_sec - tv.tv_sec) * 1000
			+ (data->b_time.tv_usec - tv.tv_usec) / 1000;
		if (time_left <= 0)
			break ;
		usleep(50);
	}
}

//Main function for each philosopher thread. Handles thinking, eating, sleeping.
static void	philosopher_step(t_id *id, t_data *data)
{
	long	delay;

	if (dead_check(data)
		|| (data->max_cycles > 0 && id->cycles >= data->max_cycles))
		return ;
	delay = id->food_delay - time_diff(data, id, 1, 0);
	if (delay > 0)
		precise_sleep(delay * 1000LL, data);
	if (dead_check(data))
		return ;
	eat_cycle(id, data);
	stamp(data, id, 3);
	precise_sleep(id->t2s * 1000LL, data);
	stamp(data, id, 4);
}

void	*run_cycle(void *arg)
{
	t_id	*id;
	t_data	*data;

	id = arg;
	data = id->data;
	wait_for_start(data);
	if (dead_check(data))
		return (NULL);
	if (id->l_lock == id->r_lock)
		return (solomen(id, data));
	if (id->id % 2 == 1 && id->cycles == 0)
	{
		stamp(data, id, 4);
		precise_sleep((id->t2e / 2) * 1000LL, data);
	}
	while (1)
	{
		philosopher_step(id, data);
		if (dead_check(data)
			|| (data->max_cycles > 0 && id->cycles >= data->max_cycles))
			break ;
	}
	return (NULL);
}
