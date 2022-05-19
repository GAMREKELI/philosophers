/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdursley <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/26 23:49:44 by pdursley          #+#    #+#             */
/*   Updated: 2022/04/26 23:51:46 by pdursley         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosopher.h"

int	ft_valid_argv(char *num)
{
	int	i;

	i = 0;
	while (num[i])
	{
		if (num[i] == ' ' || num[i] == '\t' || num[i] == '\v'
			|| num[i] == '\r' || num[i] == '\n')
		{
			i ++;
			while (!(num[i] >= '0' && num[i] <= '9'))
				i ++;
		}
		else if (num[i] >= '0' && num[i] <= '9')
			i ++;
		else if (num[i] == '+')
			i ++;
		else if (num[i] == '-')
			return (0);
		else
			return (0);
	}
	return (1);
}

int	parse(int ac, char **av)
{
	int	i;

	i = 0;
	if (ac < 5 || ac > 6)
		return (0);
	while (++ i < ac)
	{
		if (ft_valid_argv(av[i]) == 0)
			return (0);
	}
	return (1);
}
