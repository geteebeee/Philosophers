/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gorg <gorg@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 16:11:47 by gorg              #+#    #+#             */
/*   Updated: 2025/05/16 18:59:29 by gorg             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>
# include <stdlib.h>
# include <stdio.h>
# include <limits.h>
# include <unistd.h>
# include <string.h>

typedef struct s_id
{
	int				id;
	int				t2d;
	int				t2e;
	struct timeval	last_meal;
	int				t2s;
	int				cycles;
	pthread_mutex_t	*r_lock;
	pthread_mutex_t	*l_lock;
	pthread_t		thread_id;
	struct s_data	*data;
	long			food_delay;
}	t_id;

typedef struct s_data
{
	int				philos;
	int				max_cycles;
	pthread_mutex_t	*forks;
	struct timeval	b_time;
	t_id			*id;
	pthread_mutex_t	d_lock;
	int				ded;
	pthread_t		monitor;
	pthread_mutex_t	m_lock;
}	t_data;

void	stamp(t_data *data, t_id *philo, int option);
void	*solomen(t_id *id, t_data *data);
void	*run_cycle(void *arg);
int		list_data(t_data *data, char **argv, int argc);
int		time_diff(t_data *data, t_id *philo, int n, int d);
void	precise_sleep(long long ms, t_data *data);
int		ft_check(char **str, char *charset);
int		ft_atoi(const char *str);
int		ft_atoi_mod(const char *str, int *error_flag);
int		joinerofdestroyers(t_data *data, int n);
int		dead_check(t_data *data);
void	set_timer(t_data *data);
void	release_forks(t_id *philo);
void	ft_putchar_fd(char s, int fd);
int		safe_gettimeofday(struct timeval *tv, t_data *data);
void	ft_putstr_fd(char *s, int fd);
void	unlocker(t_data *data);
void	ft_putstr_fd(char *s, int fd);
int		next_deadline_ms(t_data *data);

#endif
