/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qtamaril <qtamaril@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 09:51:12 by qtamaril          #+#    #+#             */
/*   Updated: 2020/11/12 11:15:14 by qtamaril         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

void	*check_time(void *arg)
{
	t_philo		*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		if (get_millisecs() - philo->meal_time > g_options.time_to_die)
			break ;
		usleep(10);
	}
	if (!g_options.stop && philo->limit_count_eat)
	{
		print_change((philo->id) + 1, " died\n", 2);
		g_options.stop = 1;
	}
	return (NULL);
}

void	*philo_life(void *arg)
{
	t_philo		*philo;
	pthread_t	tid;

	pthread_create(&tid, NULL, check_time, arg);
	philo = (t_philo *)arg;
	while (philo->limit_count_eat)
	{
		if (g_options.stop)
			break ;
		philo_eats(philo);
		if (g_options.stop)
			break ;
		print_change(philo->id + 1, " is sleeping\n", 1);
		ft_sleep(g_options.time_to_sleep);
		if (g_options.stop)
			break ;
		print_change(philo->id + 1, " is thinking\n", 1);
		philo->limit_count_eat--;
	}
	return (NULL);
}

void	all_threads_create_join(pthread_t *tids)
{
	t_philo	philo[g_options.num_of_philo];
	int		i;

	i = 0;
	while (i < g_options.num_of_philo)
	{
		init_philo(&philo[i], i);
		pthread_create(&tids[i], NULL, philo_life, &philo[i]);
		i++;
	}
	i = 0;
	while (i < g_options.num_of_philo)
		pthread_join(tids[i++], NULL);
}

void	start(void)
{
	pthread_t		tids[g_options.num_of_philo];
	pthread_mutex_t	forks_mutexes[g_options.num_of_philo];
	int				i;

	init_forks_mutexes(forks_mutexes);
	all_threads_create_join(tids);
	i = 0;
	while (i < g_options.num_of_philo)
		pthread_mutex_destroy(&g_options.forks_mutexes[i++]);
}

int		main(int ac, char **av)
{
	if (!check_options(ac, av))
		return (1);
	start();
	pthread_mutex_destroy(&g_options.write_mutex);
	return (0);
}
