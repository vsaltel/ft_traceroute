#include "traceroute.h"

static void	print_received(t_tr *tr, char *recv_ip, char **last_ip)
{
	double	time;
	char	*name;

	name = recv_ip;
	if (tr->fqdn)
		name = tr->fqdn;
	time = tr->aft.tv_sec * 1000.0 + tr->aft.tv_usec / 1000.0;
	time = time - (tr->bef.tv_sec * 1000.0 + tr->bef.tv_usec / 1000.0);
	if (!*last_ip || ft_strcmp(recv_ip, *last_ip))
		printf(" %s (%s)  %.2f ms ", name, recv_ip, time);
	else
		printf(" %.2f ms ", time);
	if (last_ip && *last_ip)
		free(*last_ip);
	*last_ip = recv_ip;
	if (!ft_strcmp(recv_ip, tr->dest_ip))
		tr->state = 0;
}

void	recv_msg(t_tr *tr, t_recv_pckt *pckt, char **last_ip)
{
	ssize_t	ret;
	char	*recv_ip;

	ret = recvfrom(tr->sockfd, pckt, sizeof(*pckt),
			0, tr->pr.sacrecv, &tr->pr.salen);
	if (ret <= 0)
	{
		printf(" *");
		return ;
	}
	gettimeofday(&tr->aft, NULL);
	recv_ip = set_inetaddr(tr->pr.sacrecv);
	tr->fqdn = get_fqdn_info(tr->pr.sacrecv);
	print_received(tr, recv_ip, last_ip);
	if (ret > 0 && pckt->hdr.type == ICMP_ECHOREPLY)
		tr->msg_recv_count++;
	free(tr->fqdn);
}
