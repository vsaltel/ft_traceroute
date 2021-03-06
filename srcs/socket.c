#include "traceroute.h"

int	set_socket(t_tr *tr)
{
	int				sock;
	int				on;
	struct timeval	timeout;

	sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sock < 0)
	{
		dprintf(2, "ft_traceroute: fail to create socket\n");
		return (-1);
	}
	on = 1;
	setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &on, sizeof(int));
	timeout.tv_sec = tr->wait_time;
	if (tr->wait_time == 0)
		timeout.tv_usec = 1;
	else
		timeout.tv_usec = 0;
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout,
		sizeof(timeout));
	return (sock);
}
