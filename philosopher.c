#include "philosopher.h"

long int    time_ph()
{
    struct timeval  t;
    int64_t        tm;

    tm = 0;
    if (gettimeofday(&t, NULL) == -1)
        return (-1);
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
			break;
		usleep(50);
	}	
}

////////////////////////////
void	print_action(char *str, t_philo *philo)
{
	int64_t time;

	time = -1;
	time = time_ph() - philo->data->start_time;
	if ((philo->data->stop == 0) && time >= 0 && time <= 2147483647)
	{
		printf("%lld ", time);
		printf("Philo %d %s", philo->philosopher, str);
	}
}
////////////////////////////////


void    other_work(t_philo *philo)
{
    pthread_mutex_lock(&philo->data->write_mutex);
    print_action("sleeping\n", philo);
    pthread_mutex_unlock(&philo->data->write_mutex);
    ft_usleep(philo->data->time_to_sleep, philo);
    pthread_mutex_lock(&philo->data->write_mutex);
    print_action("thinking\n", philo);
    pthread_mutex_unlock(&philo->data->write_mutex);
}

void work_philo(t_philo *philo)
{
    pthread_mutex_lock(&philo->left_fork);
    pthread_mutex_lock(&philo->data->write_mutex);
    print_action("took a fork\n", philo);
    pthread_mutex_unlock(&philo->data->write_mutex);
    pthread_mutex_lock(philo->right_fork);
    pthread_mutex_lock(&philo->data->write_mutex);
    print_action("took a fork\n", philo);
    pthread_mutex_unlock(&philo->data->write_mutex);
    pthread_mutex_lock(&philo->data->write_mutex);
	pthread_mutex_lock(&philo->data->time_eat);
    print_action("is eating\n", philo);
	philo->nb_eat = time_ph();
    pthread_mutex_unlock(&philo->data->time_eat);
    pthread_mutex_unlock(&philo->data->write_mutex);
    ft_usleep(philo->data->time_to_eat, philo);
    pthread_mutex_unlock(philo->right_fork);
    pthread_mutex_unlock(&philo->left_fork);
}

void	dead_philo(t_p *p, t_philo *philo)
{
	int		i;
	
	while(philo->data->all == 0)
	{
		i = -1;
		while ((philo->data->stop == 0) && (++ i < philo->data->num_philos))
		{
			pthread_mutex_lock(&philo->data->time_eat);
			if ((time_ph() - p->ph[i].nb_eat) > philo->data->time_to_die)
			{
				pthread_mutex_lock(&philo->data->write_mutex);
				print_action("dead\n", philo);
				philo->data->stop = 1;
				pthread_mutex_unlock(&philo->data->write_mutex);
			}
			usleep(100);
			pthread_mutex_unlock(&philo->data->time_eat);
		}
		if (philo->data->stop == 1)
			break;
	}
}

void *ft_phr(void *p)
{
    t_philo *philo;

    philo = (t_philo *)p;
    if ((philo->philosopher % 2) == 0)
		usleep(1000);
	while (philo->data->stop == 0)
	{
		work_philo(philo);
		other_work(philo);

	}
    return (NULL);
}

int ft_pthread(t_p *philo)
{
    int i;

    i = -1;
    while(++ i < philo->data.num_philos)
    {
        philo->ph[i].data = &philo->data;
        if (pthread_create(&philo->ph[i].philo_id, NULL, ft_phr, &philo->ph[i]) != 0)
            return (1);
		philo->ph[i].nb_eat = time_ph();
        printf("\n%ld\n", philo->ph[i].philo_id);
    }
    return (0);
}

void    error_message(void)
{
    write(1, "Error\n", 6);
}

void    in_m(t_p *philo)
{
    pthread_mutex_init(&philo->data.dead, NULL);
    pthread_mutex_init(&philo->data.write_mutex, NULL);
    pthread_mutex_init(&philo->data.time_eat, NULL);
    pthread_mutex_init(&philo->data.finish, NULL);
    philo->data.flag = 1;
    philo->data.code_error = 1;
}

int init_mutex(t_p *philo)
{
    int i;

    i = -1;
    in_m(philo);
    if ((philo->data.start_time = time_ph()) == -1)
        return (0);
    philo->data.stop = 0;
	philo->data.all = 0;
    while (++ i < philo->data.num_philos)
    {
        philo->ph[i].philosopher = i + 1;
        philo->ph[i].nb_eat = philo->data.start_time;
        philo->ph[i].right_fork = NULL;
        philo->ph[i].finish = 0;
		philo->ph[i].last_eat = 0;
        if ((pthread_mutex_init(&philo->ph[i].left_fork, NULL)) != 0)
            return (0);
        if (i == philo->data.num_philos - 1)
            philo->ph[i].right_fork = &philo->ph[0].left_fork;
        else    
            philo->ph[i].right_fork = &philo->ph[i + 1].left_fork;      
    }
    return (1);
}

int    init_argc(t_data data, t_p *philo, int ac, char **arg)
{
    philo->data.num_philos = ft_atoi(arg[1]);
    philo->data.time_to_die = ft_atoi(arg[2]);
    philo->data.time_to_eat = ft_atoi(arg[3]);
    philo->data.time_to_sleep = ft_atoi(arg[4]);
    if (ac == 6)
    {
        philo->data.amount_philos = ft_atoi(arg[5]);
        if (philo->data.amount_philos < 2)
            return (0);
    }
    if (philo->data.num_philos < 2 || !philo->data.time_to_die || !philo->data.time_to_eat || !philo->data.time_to_sleep)
        return (0);
    return (1);
}

void ft_stop(t_p *philo)
{
    int i;

    i = -1;
    while (++ i < philo->data.num_philos)
        pthread_join(philo->ph[i].philo_id, NULL);
    pthread_mutex_destroy(&philo->data.write_mutex);
	pthread_mutex_destroy(&philo->data.dead);
	pthread_mutex_destroy(&philo->data.time_eat);
	pthread_mutex_destroy(&philo->data.finish);
    i = -1;
    while (++ i < philo->data.num_philos)
        pthread_mutex_destroy(&philo->ph[i].left_fork);
}        

int main(int argc, char *argv[])
{
    t_p philo;

    if (parse(argc, argv, &philo) == 0)
    {
        error_message();
        return (0);
    }
    if (!init_argc(philo.data, &philo, argc, argv))
    {
        error_message();
        return (1);
    }
    if (!(philo.ph = malloc(sizeof(t_philo) * philo.data.num_philos)))
    {
        error_message();
        return (1);
    }
    if (init_mutex(&philo) == 0)
    {
        return (1);
    }
    ft_pthread(&philo);
    ft_stop(&philo);
    return (0);
}
