#include "traceroute.h"

static int	check_options(t_tr *tr)
{
	if (!tr->dest_name)
		return (1);
	if (tr->ttl <= 0 || tr->ttl > 255 || tr->ttl >= tr->max_ttl)
		return (return_error("ft_traceroute: first hop out of range\n", 1));
	if (tr->datalen < 0 || tr->datalen > BUFSIZE)
		return (return_error("ft_traceroute: illegal packet size\n", 1));
	if (tr->max_ttl <= 0 || tr->max_ttl > 255)
		return (return_error("ft_traceroute: max hops cannot be more than 255\n", 1));
	if (tr->nqueries > 10)
		return (return_error("ft_traceroute: no more than 10 probes per hop\n", 1));
	return (0);
}

static int	set_option(t_tr *tr, char **av,
	int *n, int x)
{
	if (av[*n][x] == 'h')
		tr->h = 1;
	else if (av[*n][x] == 'F')
		tr->df_bit = 0;
	else if (av[*n][x] == 'f')
		tr->ttl = ft_atoi(av[++(*n)]);
	else if (av[*n][x] == 'm')
		tr->max_ttl = ft_atoi(av[++(*n)]);
	else if (av[*n][x] == 'q')
		tr->nqueries = ft_atoi(av[++(*n)]);
	else if (av[*n][x] == 't')
		tr->tos = ft_atoi(av[++(*n)]);
	else if (av[*n][x] == 'w')
		tr->wait_time = ft_atoi(av[++(*n)]);
	else
	{
		dprintf(2, "ft_traceroute: invalid option -- '%c'\n", av[*n][x]);
		return (1);
	}
	return (0);
}

static int	set_option_loop(t_tr *tr, int ac,
	char **av, int *n)
{
	int	x;

	x = 0;
	while (av[*n][++x])
	{
		if (av[*n][x] == 'f' || av[*n][x] == 'm' || av[*n][x] == 'q'
			|| av[*n][x] == 't' || av[*n][x] == 'w')
		{
			if (av[*n][x + 1])
				return (1);
			if (*n + 1 == ac)
			{
				dprintf(2, "ft_traceroute: option requires an argument -- '%c'\n",
				av[*n][x]);
				return (1);
			}
			return (set_option(tr, av, n, x));
		}
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
		{
			if (tr->datalen != TR_PKT_S)
				return (1);
			tr->datalen = ft_atoi(av[n]);
		}
		else
			tr->dest_name = ft_strdup(av[n]);
	}
	if (check_options(tr))
		return (1);
	return (0);
}
