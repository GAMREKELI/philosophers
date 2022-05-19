/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdursley <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/26 23:49:44 by pdursley          #+#    #+#             */
/*   Updated: 2022/04/26 23:51:46 by pdursley         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosopher.h"

int	in_m(t_p *philo)
{
	if ((pthread_mutex_init(&philo->data.dead, NULL)) != 0)
		return (1);
	if ((pthread_mutex_init(&philo->data.write_mutex, NULL)) != 0)
		return (1);
	if ((pthread_mutex_init(&philo->data.time_eat, NULL)) != 0)
		return (1);
	return (0);
}

int	init_mutex(t_p *philo)
{
	int	i;

	i = -1;
	philo->data.start_time = time_ph();
	if ((philo->data.start_time) == -1)
		return (2);
	if (in_m(philo))
		return (0);
	philo->data.stop = 0;
	philo->data.all = 0;
	while (++ i < philo->data.num_philos)
	{
		philo->ph[i].philosopher = i + 1;
		philo->ph[i].nb_eat = philo->data.start_time;
		philo->ph[i].right_fork = NULL;
		philo->ph[i].kol_eat = 0;
		if ((pthread_mutex_init(&philo->ph[i].left_fork, NULL)) != 0)
			return (0);
		if (i == philo->data.num_philos - 1)
			philo->ph[i].right_fork = &philo->ph[0].left_fork;
		else
			philo->ph[i].right_fork = &philo->ph[i + 1].left_fork;
	}
	return (1);
}

int	init_argc(t_p *philo, int ac, char **arg)
{
	philo->data.num_philos = ft_atoi(arg[1]);
	philo->data.time_to_die = ft_atoi(arg[2]);
	philo->data.time_to_eat = ft_atoi(arg[3]);
	philo->data.time_to_sleep = ft_atoi(arg[4]);
	if (ac == 6)
	{
		philo->data.amount_philos = ft_atoi(arg[5]);
		if (philo->data.amount_philos <= 0)
			return (0);
	}
	else
		philo->data.amount_philos = -1;
	if (philo->data.num_philos < 1 || !philo->data.time_to_die
		|| !philo->data.time_to_eat || !philo->data.time_to_sleep)
		return (0);
	return (1);
}
