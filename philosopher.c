#include "philosopher.h"

long int    time_ph()
{
    struct timeval  t;
    long int        tm;

    tm = 0;
    if (gettimeofday(&t, NULL) == -1)
        return (-1);
    tm = (t.tv_sec * 1000 + t.tv_usec / 1000);
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

int work_philo(t_philo *philo)
{
    pthread_mutex_lock(&philo->left_fork);
    pthread_mutex_lock(&philo->data->write_mutex);
    printf("%d the philosopher took a fork\n", philo->philosopher);
    pthread_mutex_unlock(&philo->data->write_mutex);
    if (!philo->right_fork)
    {
        ft_usleep(philo->data->time_to_die * 2);
    }
    pthread_mutex_lock(philo->right_fork);
    pthread_mutex_lock(&philo->data->write_mutex);
    printf("%d the philosopher took a fork\n", philo->philosopher);
    pthread_mutex_unlock(&philo->data->write_mutex);
    pthread_mutex_lock(&philo->data->write_mutex);
    printf("%d the philosopher is eating\n", philo->philosopher);
    pthread_mutex_lock(&philo->data->time_eat);
    philo->nb_eat = time_ph(philo->data);
    pthread_mutex_unlock(&philo->data->time_eat);
    pthread_mutex_unlock(&philo->data->write_mutex);
    usleep(philo->data->time_to_eat);
    pthread_mutex_unlock(philo->right_fork);
    pthread_mutex_unlock(&philo->left_fork);
}

int check_philo(t_philo *philo, int flag)
{
    pthread_mutex_lock(&philo->data->dead);
    if (flag != 0)
        philo->data->stop = flag;
    if (philo->data->stop != 0)
    {
        pthread_mutex_unlock(&philo->data->dead);
        return (1);
    }
    pthread_mutex_unlock(&philo->data->dead);
    return (0);
}

void    *dead_philo(void *p)
{
    t_philo *philo;

    philo = (t_philo *)p;
    ft_usleep(philo->data->time_to_die + 1);
    pthread_mutex_lock(&philo->data->time_eat);
    pthread_mutex_lock(&philo->data->finish);
    if (!check_philo(philo, 0) && (time_ph() - philo->data->start_time) >= philo->data->time_to_die)
    {
        pthread_mutex_unlock(&philo->data->time_eat);
        pthread_mutex_unlock(&philo->data->finish);
        pthread_mutex_lock(&philo->data->write_mutex);
        printf("%d Philo dead\n", philo->philosopher);
        pthread_mutex_unlock(&philo->data->write_mutex);
        check_philo(philo, 1);
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
    {
        ft_usleep(philo->data->time_to_eat / 10);
    }
    while (check_philo(philo, 0) == 0)
    {
        pthread_create(&philo->philo_d_id, NULL, dead_philo, p);
        work_philo(philo);
        pthread_detach(philo->philo_d_id);

    }
    return (NULL);
}

int ft_pthread(t_p *philo)
{
    int i;

    i = 0;
    while(i < philo->data.num_philos)
    {
        philo->ph[i].data = &philo->data;
        if (pthread_create(&philo->ph[i].philo_id, NULL, ft_phr, &philo->ph[i]) != 0)
            return (1);   
        i ++;    
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
}

int init_mutex(t_p *philo)
{
    int i;

    i = 0;
    in_m(philo);
    if ((philo->data.start_time = time_ph()) == -1)
        return (0);
    philo->data.stop = 0;
    while (i < philo->data.num_philos)
    {
        philo->ph[i].philo_id = i + 1;
        philo->ph[i].philosopher = i + 1;
        philo->ph[i].nb_eat = 0;
        philo->ph[i].right_fork = NULL;
        philo->ph[i].finish = 0;
        if ((pthread_mutex_init(&philo->ph[i].left_fork, NULL)) != 0)
            return (0);
        if (i == philo->data.num_philos - 1)
            philo->ph[i].right_fork = &philo->ph[0].left_fork;
        else    
            philo->ph[i].right_fork = &philo->ph[i + 1].left_fork;
        i ++;      
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

int ft_damage(t_p *philo)
{
    pthread_mutex_lock(&philo->data.dead);
    if (philo->data.stop != 0)
    {
        pthread_mutex_unlock(&philo->data.dead);
        return (1);
    }
    pthread_mutex_unlock(&philo->data.dead);
    return (0);
}

void ft_stop(t_p *philo)
{
    int i;

    i = 0;
    while ((ft_damage(philo)) == 0)
        usleep(1);
    while (i < philo->data.num_philos)
    {
        pthread_join(philo->ph[i].philo_id, NULL);
        i ++;
    }
    pthread_mutex_destroy(&philo->data.write_mutex);
    i = 0;
    while (i < philo->data.num_philos)
    {
        pthread_mutex_destroy(&philo->ph[i].left_fork);
        i ++;
    }
}

int main(int argc, char *argv[])
{
    t_p philo;

    if (argc != 5)
    {
        error_message();
        return (1);
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