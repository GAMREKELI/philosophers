#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

typedef pthread_mutex_t t_mutex;

typedef struct p_philo
{
    pthread_t       philo_id;
    pthread_t       philo_d_id;
    t_mutex         left_fork;
    t_mutex         *right_fork;
    int             flag;
    long int        nb_eat;
    int             finish;
    int             philosopher;

    struct p_data   *data;

}               t_philo;

typedef struct  p_data
{
    t_mutex *p_mutex;
    int     *num;


    int             num_philos;
    int             time_to_die;
    int             time_to_eat;
    int             time_to_sleep;
    long int        start_time;
    t_mutex         dead;
    t_mutex         time_eat;
    t_mutex         finish;
    t_mutex         write_mutex;
    int             stop;

}               t_data;

typedef struct p_p
{
    t_data  data;
    t_philo *ph;
}               t_p;


int	ft_atoi(const char *str);

#endif