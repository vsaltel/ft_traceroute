#include "traceroute.h"

static void	read_one(t_tr *tr, char *sbuf, t_recv_pckt *r_pckt)
{
	char			*last_ip;
	unsigned int	n;

	last_ip = NULL;
	n = 0;
	printf("%2d ", tr->ttl);
	while (n < tr->nqueries)
	{
		send_msg(sbuf);
		recv_msg(tr, r_pckt, &last_ip);
		n++;
	}
	printf("\n");
	free(last_ip);
	last_ip = NULL;
}

static int	read_loop(t_tr *tr)
{
	t_recv_pckt	r_pckt;
	t_send_pckt	*s_pckt;
	char		sbuf[BUFSIZE];

	s_pckt = (t_send_pckt *)sbuf;
	tr->state = 1;
	signal(SIGINT, &catch_sigint);
	set_send_pckt(s_pckt);
	tr->sockfd = set_socket(tr);
	while (tr->ttl <= tr->max_ttl && tr->state && tr->count_max)
	{
		if (!tr->sockfd || tr->sockfd < 0)
			return (-4);
		s_pckt->ip.ip_ttl = tr->ttl;
		read_one(tr, sbuf, &r_pckt);
		tr->count_max--;
		tr->ttl++;
	}
	return (0);
}

static struct addrinfo	*get_addr_info(t_tr *tr)
{
	struct addrinfo	*info;

	info = reverse_dns_info(tr->dest_name, NULL, AF_INET, 0);
	if (!info)
		return (NULL);
	tr->dest_ip = set_inetaddr(info->ai_addr);
	printf("ft_traceroute to %s (%s), %d hops max, %d byte packets\n",
		info->ai_canonname ? info->ai_canonname : tr->dest_name,
		tr->dest_ip, tr->max_ttl, tr->datalen);
	if (info->ai_family != AF_INET)
		return (NULL);
	return (info);
}

int	traceroute(t_tr *tr)
{
	struct addrinfo	*info;
	int				ret;

	info = get_addr_info(tr);
	if (!info)
		return (-2);
	tr->pr.sasend = info->ai_addr;
	tr->pr.sacrecv = malloc(info->ai_addrlen);
	ft_bzero(tr->pr.sacrecv, info->ai_addrlen);
	tr->pr.salen = info->ai_addrlen;
	ret = read_loop(tr);
	freeaddrinfo(info);
	return (ret);
}
