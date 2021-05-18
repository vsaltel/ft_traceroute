#include "traceroute.h"

void	set_send_pckt(t_send_pckt *pckt)
{
	int	i;

	ft_bzero(pckt, BUFSIZE);
	i = -1;
	while (++i < g_tr.datalen)
		pckt->msg[i] = (char)i;
	pckt->ip.ip_v = 4;
	pckt->ip.ip_hl = 5;
	pckt->ip.ip_tos = g_tr.tos;
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
	pckt->icmp.icmp_cksum = checksum((u_short *)&pckt->icmp,
			sizeof(struct icmp) + g_tr.datalen);
}

void	send_msg(void *pckt)
{
	gettimeofday(&g_tr.bef, NULL);
	sendto(g_tr.sockfd, pckt,
		sizeof(struct ip) + sizeof(struct icmp) + g_tr.datalen,
		0, g_tr.pr.sasend, g_tr.pr.salen);
	g_tr.msg_sent++;
}
