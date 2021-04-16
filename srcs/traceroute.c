#include "traceroute.h"

static void	set_send_pckt(t_send_pckt *pckt)
{
	int	i;

	ft_printf("%p %p %p\n", &pckt->ip, &pckt->icmp, &pckt->msg);
	//ft_bzero(pckt->msg, BUFSIZE - sizeof(struct icmp) - sizeof(struct ip));
	i = -1;
	while (++i < g_tr.datalen)
		pckt->msg[i] = (char)i;
	pckt->ip.ip_v = 4;
	pckt->ip.ip_hl = 5;
	pckt->ip.ip_tos = 0;
	pckt->ip.ip_len = sizeof(struct ip) + sizeof(struct icmp);
	pckt->ip.ip_id = g_tr.pid;
	pckt->ip.ip_off = g_tr.df_bit;
	pckt->ip.ip_p = IPPROTO_ICMP;
	pckt->ip.ip_sum = 0;
	pckt->ip.ip_dst = (struct in_addr)
		((struct sockaddr_in *)g_tr.pr.sasend)->sin_addr;
	pckt->ip.ip_ttl = 0;
	pckt->icmp.icmp_type = ICMP_ECHO;
	pckt->icmp.icmp_code = 0;
	pckt->icmp.icmp_id = g_tr.pid;
	pckt->icmp.icmp_seq = g_tr.msg_sent;
	pckt->icmp.icmp_cksum = 0;
	pckt->icmp.icmp_cksum = checksum((u_short *)&pckt->icmp, sizeof(struct icmp) + g_tr.datalen);
}

static int	read_loop(t_tr *tr)
{
	t_recv_pckt	r_pckt;
	char		sbuf[BUFSIZE];
	t_send_pckt *s_pckt;

	s_pckt = (t_send_pckt *)sbuf;	
	tr->state = 1;
	signal(SIGINT, &catch_sigint);
	set_send_pckt(s_pckt);
	tr->sockfd = set_socket(tr);
	while (tr->ttl < tr->max_ttl && tr->state && tr->count_max)
	{
		if (!tr->sockfd || tr->sockfd < 0)
			return (-4);
		s_pckt->ip.ip_ttl = tr->ttl;
		send_msg(sbuf);
		ft_bzero(&r_pckt, sizeof(r_pckt));
		recv_msg(tr, &r_pckt);
		tr->msg_count++;
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
	ft_printf("ft_traceroute to %s (%s), %d hops max, %d byte packets\n",
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
