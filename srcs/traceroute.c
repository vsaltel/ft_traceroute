#include "traceroute.h"

static int	read_loop(t_tr *tr)
{
	t_tr_pckt	pckt;

	tr->state = 1;
	signal(SIGINT, &catch_sigint);
	while (tr->ttl < TTL_MAX && tr->state && tr->count_max)
	{
		tr->sockfd = set_socket(tr);
		if (!tr->sockfd || tr->sockfd < 0)
			return (-4);
		send_msg();
		ft_bzero(&pckt, sizeof(pckt));
		recv_msg(tr, &pckt);
		tr->msg_count++;
		tr->count_max--;
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
	ft_printf("FT_TRACEROUTE %s (%s) %d(%d) data bytes\n",
		info->ai_canonname ? info->ai_canonname : tr->dest_name,
		tr->dest_ip, tr->datalen,
		tr->datalen + sizeof(struct iphdr) + sizeof(struct icmphdr));
	if (info->ai_family != AF_INET)
		return (NULL);
	return (info);
}

int	traceroute(t_tr *tr)
{
	struct addrinfo	*info;
	int				sock;
	int				ret;

	gettimeofday(&tr->launch_time, NULL);
	tr->bef = tr->launch_time;
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
