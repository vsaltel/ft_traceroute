#include "traceroute.h"
/*
void	send_msg(void)
{
	int			len;
	int			ret;
	struct icmp *icmp;
	char		sendbuf[BUFSIZE];

	icmp = (struct icmp *)sendbuf;
	icmp->icmp_type = ICMP_ECHO;
	icmp->icmp_code = 0;
	icmp->icmp_id = g_tr.pid;
	icmp->icmp_seq = g_tr.msg_sent;
	ft_memset(icmp->icmp_data, 0xa5, g_tr.datalen);
	gettimeofday((struct timeval *)icmp->icmp_data, NULL);
	len = 8 + g_tr.datalen;
	icmp->icmp_cksum = 0;
	icmp->icmp_cksum = checksum((u_short *) icmp, len);
	gettimeofday(&g_tr.bef, NULL);
	ret = sendto(g_tr.sockfd, sendbuf, len, 0,
		g_tr.pr.sasend, g_tr.pr.salen);
	if (ret >= 0)
		g_tr.msg_sent++;
}
*/
void	send_msg(t_send_pckt *pckt)
{
	int			ret;

	gettimeofday(&g_tr.bef, NULL);
	ret = sendto(g_tr.sockfd, pckt, sizeof(struct ip) + sizeof(struct icmp) + g_tr.datalen, 0,
		g_tr.pr.sasend, g_tr.pr.salen);
	if (ret == -1)
		ft_printf("ret = %d, %s\n", ret, strerror(errno));
	ft_printf("ret = %d\n", ret);
	if (ret >= 0)
		g_tr.msg_sent++;
}
