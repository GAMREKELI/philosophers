/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   units.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdursley <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/26 23:49:44 by pdursley          #+#    #+#             */
/*   Updated: 2022/04/26 23:51:46 by pdursley         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"
int	ft_check(unsigned long result, int sign)
{
	if (result >= 9223372036854775807 && sign > 0)
		return (-1);
	if (result >= 9223372036854775807 && sign < 0)
		return (0);
	return (1);
}

int	ft_atoi(const char *str)
{
	int				i;
	unsigned long	result;
	int				sign;

	i = 0;
	result = 0;
	sign = 1;
	while (str[i] == '\n' || str[i] == '\f' || str[i] == '\t'
		|| str[i] == '\r' || str[i] == '\v' || str[i] == ' ')
		i ++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -sign;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	if (ft_check(result, sign) != 1)
		return (ft_check(result, sign));
	return (result * sign);
}