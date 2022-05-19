/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdursley <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/26 23:49:44 by pdursley          #+#    #+#             */
/*   Updated: 2022/04/26 23:51:46 by pdursley         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosopher.h"

void	other_work(t_philo *philo)
{
	print_action("is sleeping\n", philo);
	ft_usleep(philo->data->time_to_sleep, philo);
	print_action("is thinking\n", philo);
}

void	work_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->left_fork);
	print_action("has taken a fork\n", philo);
	if (!philo->right_fork)
	{
		ft_usleep(philo->data->time_to_die * 2, philo);
		return ;
	}
	pthread_mutex_lock(philo->right_fork);
	print_action("has taken a fork\n", philo);
	pthread_mutex_lock(&philo->data->time_eat);
	print_action("is eating\n", philo);
	philo->nb_eat = time_ph();
	pthread_mutex_unlock(&philo->data->time_eat);
	ft_usleep(philo->data->time_to_eat, philo);
	(philo->kol_eat)++;
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(&philo->left_fork);
}

void	*ft_phr(void *p)
{
	t_philo	*philo;

	philo = (t_philo *)p;
	if ((philo->philosopher % 2) == 0)
		usleep(600);
	while (philo->data->stop == 0)
	{
		work_philo(philo);
		if (philo->data->all)
			break ;
		other_work(philo);
	}
	return (NULL);
}

int	ft_pthread(t_p *philo)
{
	int	i;

	i = -1;
	while (++i < philo->data.num_philos)
	{
		philo->ph[i].data = &philo->data;
		if (pthread_create(&philo->ph[i].philo_id, NULL, \
			ft_phr, &philo->ph[i]) != 0)
		{
			error_message(5);
			return (1);
		}
		philo->ph[i].nb_eat = time_ph();
		if (philo->ph[i].nb_eat == -1)
			return (1);
	}
	dead_philo(philo, philo->ph);
	return (0);
}
