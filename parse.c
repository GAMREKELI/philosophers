#include "philosopher.h"

int	ft_valid_argv(char *num, t_p *philo)
{
	int i;

	i = 0;
	while (num[i])
	{
		if (num[i] == ' ' || num[i] == '\t' || num[i] == '\v'
			|| num[i] == '\r' || num[i] == '\n')
		{
			philo->num_argc ++;
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
	philo->num_argc ++;
	return (1);
}

int	parse(int ac, char **av, t_p *philo)
{
	int i;

	i = 0;
	philo->num_argc = 0;
	while (++ i < ac)
	{
		if (ft_valid_argv(av[i], philo) == 0)
			return (0);
	}
	printf("\n\n%d\n\n", philo->num_argc);
	return (1);
}
