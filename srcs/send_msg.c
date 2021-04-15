#include "traceroute.h"

void	send_msg(void)
{
	int			len;
	int			ret;
	t_send_pckt	*pckt;
	char		sendbuf[BUFSIZE];

	pckt = (t_send_pckt *)sendbuf;

	pckt->ip.ip_v = 4;
	pckt->ip.ip_hl = 5;
	pckt->ip.ip_tos = 0;
	pckt->ip.ip_len = sizeof(t_send_pckt);
	pckt->ip.ip_id = g_tr.pid;
	pckt->ip.ip_off = g_tr.df_bit;
	pckt->ip.ip_p = IPPROTO_ICMP;
	pckt->ip.ip_sum = 0;
	pckt->ip.ip_dst = (struct in_addr)((struct sockaddr_in *)g_tr.pr.sasend)->sin_addr;
	pckt->ip.ip_ttl = 0;


	pckt->icmp.icmp_type = ICMP_ECHO;
	pckt->icmp.icmp_code = 0;
	pckt->icmp.icmp_id = g_tr.pid;
	pckt->icmp.icmp_seq = g_tr.msg_sent;
	ft_memset(pckt->icmp.icmp_data, 0xa5, g_tr.datalen);
	gettimeofday((struct timeval *)pckt->icmp.icmp_data, NULL);
	len = 8 + g_tr.datalen;
	pckt->icmp.icmp_cksum = 0;
	pckt->icmp.icmp_cksum = checksum((u_short *) &pckt->icmp, len);
	gettimeofday(&g_tr.bef, NULL);
	ret = sendto(g_tr.sockfd, sendbuf, len, 0,
			g_tr.pr.sasend, g_tr.pr.salen);
	if (ret)
		g_tr.msg_sent++;
}
