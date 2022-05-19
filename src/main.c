/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdursley <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/26 23:49:44 by pdursley          #+#    #+#             */
/*   Updated: 2022/04/26 23:51:46 by pdursley         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosopher.h"

void	ft_stop(t_p *philo)
{
	int	i;

	i = -1;
	while (++ i < philo->data.num_philos)
	{
		pthread_join(philo->ph[i].philo_id, NULL);
	}
	pthread_mutex_destroy(&philo->data.dead);
	pthread_mutex_destroy(&philo->data.time_eat);
	i = -1;
	while (++ i < philo->data.num_philos)
	{
		pthread_mutex_destroy(&philo->ph[i].left_fork);
	}
	pthread_mutex_destroy(&philo->data.write_mutex);
}

int	ft_prog(int argc, char *argv[], t_p *philo)
{
	if (!parse(argc, argv))
	{
		error_message(1);
		return (1);
	}
	if (!init_argc(philo, argc, argv))
	{
		error_message(1);
		return (1);
	}
	return (0);
}

int	main(int argc, char *argv[])
{
	t_p	philo;

	if (ft_prog(argc, argv, &philo))
		return (1);
	philo.ph = malloc(sizeof(t_philo) * philo.data.num_philos);
	if (!philo.ph)
	{
		error_message(2);
		return (1);
	}
	if (!(init_mutex(&philo)))
	{
		error_message(4);
		return (1);
	}
	ft_pthread(&philo);
	ft_stop(&philo);
	free(philo.ph);
	return (0);
}
