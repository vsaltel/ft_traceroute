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
	if (ret == -1)
		ft_printf("ret = %d, %s\n", ret, strerror(errno));
	ft_printf("ret = %d\n", ret);
	if (ret >= 0)
		g_tr.msg_sent++;
}
*/
void	send_msg(void)
{
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
	pckt->ip.ip_ttl = g_tr.ttl;

	pckt->icmp.icmp_type = ICMP_ECHO;
	pckt->icmp.icmp_code = 0;
	pckt->icmp.icmp_id = g_tr.pid;
	pckt->icmp.icmp_seq = g_tr.msg_sent;
	pckt->icmp.icmp_cksum = 0;
	pckt->icmp.icmp_cksum = checksum((u_short *) &pckt->icmp, struct(icmp) + g_tr.datalen);

	int	i;
	i = -1;
	while (++i < g_tr.datalen)
		pckt->msg[i] = (char)i;


	gettimeofday(&g_tr.bef, NULL);
	ret = sendto(g_tr.sockfd, sendbuf, sizeof(struct ip) + sizeof(struct icmp) + g_tr.datalen, 0,
		g_tr.pr.sasend, g_tr.pr.salen);
	if (ret == -1)
		ft_printf("ret = %d, %s\n", ret, strerror(errno));
	ft_printf("ret = %d\n", ret);
	if (ret >= 0)
		g_tr.msg_sent++;
}
