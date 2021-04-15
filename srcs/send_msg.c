#include "traceroute.h"

void	send_msg(void)
{
	int			len;
	int			ret;
	t_send_pckt	*pckt;
	char		sendbuf[BUFSIZE];

	pckt = (t_send_pckt *)sendbuf;
	pckt->icmp->icmp_type = ICMP_ECHO;
	pckt->icmp->icmp_code = 0;
	pckt->icmp->icmp_id = g_tr.pid;
	pckt->icmp->icmp_seq = g_tr.msg_sent;
	ft_memset(icmp->icmp_data, 0xa5, g_tr.datalen);
	gettimeofday((struct timeval *)pckt->icmp->icmp_data, NULL);
	len = 8 + g_tr.datalen;
	pckt->icmp->icmp_cksum = 0;
	pckt->icmp->icmp_cksum = checksum((u_short *) pckt->icmp, len);
	gettimeofday(&g_tr.bef, NULL);
	ret = sendto(g_tr.sockfd, sendbuf, len, 0,
			g_tr.pr.sasend, g_tr.pr.salen);
	if (ret)
		g_tr.msg_sent++;
}
