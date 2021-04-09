#include "traceroute.h"

static int	check_options(t_tr *tr)
{
	if (!tr->dest_name)
		return (1);
	return (0);
}

static int	set_option(t_tr *tr, char **av,
	int *n, int x)
{
	if (av[*n][x] == 'h')
		tr->h = 1;
	else
	{
		ft_dprintf(2, "ft_traceroute: invalid option -- '%c'\n", av[*n][x]);
		return (1);
	}
	return (0);
}

static int	set_option_loop(t_tr *tr,
	char **av, int *n)
{
	int	x;

	x = 0;
	while (av[*n][++x])
	{
		if (set_option(tr, av, n, x))
			return (1);
	}
	return (0);
}

int	get_args(t_tr *tr, int ac, char **av)
{
	int	n;

	n = 0;
	while (++n < ac)
	{
		if (av[n][0] && av[n][0] == '-')
		{
			if (set_option_loop(tr, ac, av, &n))
				return (1);
		}
		else if (tr->dest_name)
			return (1);
		else
			tr->dest_name = strdup(av[n]);
	}
	if (check_options(tr))
		return (1);
	return (0);
}
