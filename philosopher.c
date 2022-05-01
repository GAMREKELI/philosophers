#include "philosopher.h"

// typedef pthread_mutex_t t_mutex;

// typedef struct  p_data
// {
//     t_mutex *p_mutex;
//     int     *num;
// }           t_data;


// void    *ft_func(void *data)
// {
//     t_data  *l_data;
//     l_data = data;
//     pthread_mutex_lock(l_data->p_mutex);
//     while(*(l_data->num) < 100)
//     {
//         (*l_data->num) ++;
//         usleep(100);
//         pthread_mutex_unlock(l_data->p_mutex);
//         pthread_mutex_lock(l_data->p_mutex);
//     }
//     pthread_mutex_unlock(l_data->p_mutex);
//     return(NULL);
// }

// int main()
// {
//     pthread_t   tmp;
//     t_mutex mutex;
//     int nu;
//     t_data  data;

//     data.p_mutex = &mutex;
//     nu = 0;
//     data.num = &nu;
//     pthread_mutex_init(&mutex, NULL);
//     pthread_create(&tmp, NULL, ft_func, &data);
//     pthread_mutex_lock(&mutex);
//     while(*(data.num) < 100)
//     {
//         printf("%d\n", *(data.num));
//         pthread_mutex_unlock(&mutex);
//         pthread_mutex_lock(&mutex);
//     }
//     pthread_mutex_unlock(&mutex);
//     pthread_join(tmp, NULL);
//     return(0);
// }

int check_philo(t_philo *philo, int flag)
{
    pthread_mutex_lock(&philo->data->dead);
    if (flag == 0)
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
    pthread_mutex_lock(&philo->data->time_eat);
    pthread_mutex_lock(&philo->data->finish);
    if ((check_philo(philo, philo->flag) == 0))
    {
        pthread_mutex_ulock(&philo->data->time_eat);
        pthread_mutex_ulock(&philo->data->finish);
        pthread_mutex_lock(&philo->data->write_mutex);
        // write(1, "Philo dead\n", 11);
        pthread_mutex_ulock(&philo->data->write_mutex);
        check_philo(philo, 0);
    }
    pthread_mutex_ulock(&philo->data->time_eat);
    pthread_mutex_ulock(&philo->data->finish);
    return (NULL);
}

void *ft_phr(void *p)
{
    t_philo *philo;

    philo = (t_philo *)p;
    if (philo->philo_id % 2 == 0)
        usleep(100 * 1000);
    while (check_philo(philo, philo->flag) == 0)
    {
        pthread_create(&philo->philo_d_id, NULL, dead_philo, p);
    }
    return (NULL);
}

int ft_pthread(t_p *philo)
{
    int i;

    i = -1;
    philo->ph->flag = 1;
    while((++ i) != philo->data.num_philos)
    {
        philo->ph[i].data = &philo->data;
        if ((pthread_create(&philo->ph[i].philo_id, NULL, ft_phr, NULL)) != 0)
            return (1);
    }
    return (0);
}

long int    time_ph(t_data *data)
{
    struct timeval  t;
    long int        tm;

    tm = 0;
    if (gettimeofday(&t, NULL) == -1)
        return (-1);
    tm = ((t.tv_sec * 1000) + (t.tv_sec / 1000));
    printf("\n%ld\n", tm);
    return (tm);
}

void    error_message(void)
{
    write(1, "Error\n", 6);
}

void    in_m(t_p *philo)
{
    pthread_mutex_init(&philo->data.dead, NULL);
    pthread_mutex_init(&philo->data.time_eat, NULL);
    pthread_mutex_init(&philo->data.finish, NULL);
}

int init_mutex(t_p *philo)
{
    int num;

    num = -1;
    in_m(philo);
    if ((philo->data.start_time = time_ph(&philo->data)) == -1)
        return (0);
    while ((++ num) != philo->data.num_philos)
    {
        philo->ph[num].philo_id = num + 1;
        philo->ph[num].nb_eat = 0;
        if ((pthread_mutex_init(&philo->ph[num].left_fork, NULL)) != 0)
            return (0);
        if (num == philo->data.num_philos - 1)
            philo->ph[num].right_fork = &philo->ph[0].left_fork;
        else    
            philo->ph[num].right_fork = &philo->ph[num + 1].left_fork;
        printf("%d\n", philo->ph[num].philo_id);
        printf("\n");
        printf("left:%d\n", philo->ph[num].left_fork);
        printf("right:%d\n", philo->ph[num].right_fork);    
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
    return (0);
}