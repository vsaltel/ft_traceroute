#include "traceroute.h"

void	send_msg(void)
{
	int			len;
	int			ret;
	struct icmp	*icmp;
	char		sendbuf[BUFSIZE];

	icmp = (struct icmp *)sendbuf;
	icmp->icmp_type = ICMP_ECHO;
	icmp->icmp_code = 0;
	icmp->icmp_id = g_tr.pid;
	icmp->icmp_seq = g_tr.msg_sent;
	ft_memset(icmp->icmp_data, 0xa5, g_tr.datalen);
	gettimeofday(&g_tr.bef, NULL);
	gettimeofday((struct timeval *)icmp->icmp_data, NULL);
	len = 8 + g_tr.datalen;
	icmp->icmp_cksum = 0;
	icmp->icmp_cksum = checksum((u_short *) icmp, len);
	ret = sendto(g_tr.sockfd, sendbuf, len, 0,
			g_tr.pr.sasend, g_tr.pr.salen);
	if (ret)
		g_tr.msg_sent++;
}
