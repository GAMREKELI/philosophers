/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdursley <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/26 23:49:44 by pdursley          #+#    #+#             */
/*   Updated: 2022/04/26 23:51:46 by pdursley         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosopher.h"

long int	time_ph(void)
{
	struct timeval	t;
	int64_t			tm;

	tm = 0;
	if (gettimeofday(&t, NULL) == -1)
	{
		error_message(3);
		return (-1);
	}
	tm = (t.tv_sec * 1000) + (t.tv_usec / 1000);
	return (tm);
}

void	ft_usleep(int64_t time_in_ms, t_philo *philo)
{
	int64_t	start_time;

	start_time = 0;
	start_time = time_ph();
	while (philo->data->stop == 0)
	{
		if ((time_ph() - start_time) >= time_in_ms)
			break ;
		usleep(50);
	}
}
