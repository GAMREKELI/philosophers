#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

typedef pthread_mutex_t	t_mutex;

typedef struct p_philo
{
	pthread_t		philo_id;
	t_mutex			left_fork;
	t_mutex			*right_fork;
	int64_t			nb_eat;
	int				philosopher;
	int				kol_eat;
	struct p_data	*data;
}				t_philo;

typedef struct p_data
{
	int				num_philos;
	int				amount_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int64_t			start_time;
	t_mutex			dead;
	t_mutex			time_eat;
	t_mutex			write_mutex;
	int				stop;
	int				all;
}				t_data;

typedef struct p_p
{
	t_data	data;
	t_philo	*ph;
}				t_p;

int			ft_atoi(const char *str);
int			parse(int ac, char **av);
void		error_message(int error);
void		dead_philo(t_p *p, t_philo *philo);
int			init_argc(t_p *philo, int ac, char **arg);
int			init_mutex(t_p *philo);
int			ft_pthread(t_p *philo);
void		print_action(char *str, t_philo *philo);
long int	time_ph(void);
void		ft_usleep(int64_t time_in_ms, t_philo *philo);
int			ft_valid_argv(char *num);

#endif
