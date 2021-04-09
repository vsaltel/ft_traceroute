#include "traceroute.h"

static void	set_rtt(t_tr *tr, double time)
{
	if (tr->rtt_min > time || tr->rtt_min == -1)
		tr->rtt_min = time;
	if (tr->rtt_max < time || tr->rtt_max == -1)
		tr->rtt_max = time;
	tr->rtt_sum += time;
	tr->rtt_sum_sq += time * time;
}

static void	print_received(t_tr *tr, t_tr_pckt *pckt,
	long recv_bytes, char *recv_ip)
{
	double	time;
	char	*name;

	name = recv_ip;
	if (tr->fqdn)
		name = tr->fqdn;
	time = tr->aft.tv_sec * 1000.0 + tr->aft.tv_usec / 1000.0;
	time = time - (tr->bef.tv_sec * 1000.0 + tr->bef.tv_usec / 1000.0);
	set_rtt(tr, time);
	ft_printf("%2d %s (%s) %.2f ms\n", tr->ttl, name, recv_ip, time);
	if (pckt->hdr.type == ICMP_ECHOREPLY)
		tr->state = 0;
}

static void	print_non_received(t_tr *tr, t_tr_pckt *pckt,
	long recv_bytes, char *recv_ip)
{
	char	*name;

	name = recv_ip;
	if (tr->fqdn)
		name = tr->fqdn;
	if (!tr->q)
		ft_printf(" %ld bytes from %s (%s): type = %d, code = %d\n",
			recv_bytes, name, recv_ip, pckt->hdr.type, pckt->hdr.code);
	tr->state = 0;
}

void	recv_msg(t_tr *tr, t_tr_pckt *pckt)
{
	ssize_t	ret;
	char	*recv_ip;
	long	recv_bytes;

	ret = recvfrom(tr->sockfd, pckt, sizeof(*pckt),
			0, tr->pr.sacrecv, &tr->pr.salen);
	if (ret <= 0)
		ft_printf("%2d -> *\n", tr->ttl);
	gettimeofday(&tr->aft, NULL);
	recv_ip = set_inetaddr(tr->pr.sacrecv);
	tr->fqdn = get_fqdn_info(tr->pr.sacrecv);
	recv_bytes = ret - sizeof(pckt->ip);
	if (ret <= 0 || pckt->hdr.code != 0)
		print_non_received(tr, pckt, recv_bytes, recv_ip);
	else
		print_received(tr, pckt, recv_bytes, recv_ip);
	if (ret > 0 && pckt->hdr.type == ICMP_ECHOREPLY)
		tr->msg_recv_count++;
	free(recv_ip);
	free(tr->fqdn);
}
