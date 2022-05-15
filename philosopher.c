#include "philosopher.h"

long int    time_ph()
{
    struct timeval  t;
    long int        tm;

    tm = 0;
    if (gettimeofday(&t, NULL) == -1)
        return (-1);
    tm = (t.tv_sec * 1000) + (t.tv_usec / 1000);
    return (tm);
}

void	ft_usleep(long int time_in_ms)
{
	long int	start_time;

	start_time = 0;
	start_time = time_ph();
	while ((time_ph() - start_time) < time_in_ms)
		usleep(time_in_ms / 10);
}

int    m_dead(t_philo *philo, int stop)
{
    pthread_mutex_lock(&philo->data->dead);
    if (stop != 1)
        philo->data->flag = -1;
    if (philo->data->flag < 0)
    {
        pthread_mutex_unlock(&philo->data->dead);
        return (0);
    }
    pthread_mutex_unlock(&philo->data->dead);
    return (1);
}

////////////////////////////
void	print_action(char *str, t_philo *philo)
{

	if (m_dead(philo, 1))
		printf("Philo %d %s", philo->philosopher, str);
}
////////////////////////////////


void    other_work(t_philo *philo)
{
    pthread_mutex_lock(&philo->data->write_mutex);
    print_action("sleeping\n", philo);
    pthread_mutex_unlock(&philo->data->write_mutex);
    ft_usleep(philo->data->time_to_sleep);
    pthread_mutex_lock(&philo->data->write_mutex);
    print_action("thinking\n", philo);
    pthread_mutex_unlock(&philo->data->write_mutex);
}

int work_philo(t_philo *philo)
{
    pthread_mutex_lock(&philo->left_fork);
    pthread_mutex_lock(&philo->data->write_mutex);
    print_action("took a fork\n", philo);
    pthread_mutex_unlock(&philo->data->write_mutex);
    if (!philo->right_fork)
    {
        ft_usleep(philo->data->time_to_die * 2);
        return (0);
    }
    pthread_mutex_lock(philo->right_fork);
    pthread_mutex_lock(&philo->data->write_mutex);
    print_action("took a fork\n", philo);
    pthread_mutex_unlock(&philo->data->write_mutex);
    pthread_mutex_lock(&philo->data->write_mutex);
    print_action("is eating\n", philo);
    pthread_mutex_lock(&philo->data->time_eat);
    philo->nb_eat = time_ph();
    pthread_mutex_unlock(&philo->data->time_eat);
    pthread_mutex_unlock(&philo->data->write_mutex);
    ft_usleep(philo->data->time_to_eat);
    pthread_mutex_unlock(philo->right_fork);
    pthread_mutex_unlock(&philo->left_fork);
    other_work(philo);
}

void    *dead_philo(void *p)
{
    t_philo *philo;

    philo = (t_philo *)p;
    ft_usleep(philo->data->time_to_die + 1);
    pthread_mutex_lock(&philo->data->time_eat);
    pthread_mutex_lock(&philo->data->finish);
    if (m_dead(philo, 1) && ((time_ph() - philo->nb_eat) >= (long)philo->data->time_to_die) && !philo->finish)
    {
        pthread_mutex_unlock(&philo->data->time_eat);
        pthread_mutex_unlock(&philo->data->finish);
        pthread_mutex_lock(&philo->data->write_mutex);
        print_action("dead\n", philo);
        philo->data->stop = 1;
        pthread_mutex_unlock(&philo->data->write_mutex);
        m_dead(philo, -1);
    }
    pthread_mutex_unlock(&philo->data->time_eat);
    pthread_mutex_unlock(&philo->data->finish);
    return (NULL);
}

void *ft_phr(void *p)
{
    t_philo *philo;

    philo = (t_philo *)p;
    if (philo->philo_id % 2 == 0)
        // usleep(philo->data->time_to_eat / 10);
        ft_usleep(philo->data->time_to_eat / 10);
    while (m_dead(philo, 1))
    {
        if (pthread_create(&philo->philo_d_id, NULL, dead_philo, p) != 0)
        {
            philo->data->code_error = -1;
            return (NULL);
        }
        work_philo(philo);
        if (pthread_detach(philo->philo_d_id) != 0)
        {
            philo->data->code_error = -1;
            return (NULL);
        }
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
    while (++ i < philo->data.num_philos)
    {
        philo->ph[i].philo_id = i + 1;
        philo->ph[i].philosopher = i + 1;
        philo->ph[i].nb_eat = philo->data.start_time;
        philo->ph[i].right_fork = NULL;
        philo->ph[i].finish = 0;
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
