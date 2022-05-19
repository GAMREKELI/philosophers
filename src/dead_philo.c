/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dead_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdursley <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/26 23:49:44 by pdursley          #+#    #+#             */
/*   Updated: 2022/04/26 23:51:46 by pdursley         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosopher.h"

void	dead_philo(t_p *p, t_philo *philo)
{
	int	i;

	while (p->data.all == 0)
	{
		i = -1;
		usleep(p->data.time_to_die + 1);
		while (p->data.stop == 0 && (++ i < p->data.num_philos))
		{
			pthread_mutex_lock(&p->data.time_eat);
			if ((time_ph() - p->ph[i].nb_eat) > p->data.time_to_die)
			{
				print_action("dead\n", philo);
				p->data.stop = 1;
			}
			pthread_mutex_unlock(&p->data.time_eat);
		}
		if (p->data.stop == 1)
			break ;
		i = 0;
		while (++ i < philo->data->num_philos && \
				p->ph[i].kol_eat >= philo->data->amount_philos && \
				philo->data->amount_philos != -1)
			i ++;
		if (i == philo->data->num_philos)
			philo->data->all = 1;
	}
}
