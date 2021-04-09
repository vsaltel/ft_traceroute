#include "traceroute.h"

int	set_socket(t_tr *tr)
{
	int	sock;
	int	size;

	sock = socket(tr->pr.sasend->sa_family, SOCK_RAW, IPPROTO_ICMP);
	if (sock < 0)
	{
		ft_dprintf(2, "ft_traceroute: fail to create socket\n");
		return (-1);
	}
	size = 60 * 1024;
	setsockopt (sock, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));
	tr->ttl++;
	setsockopt(sock, IPPROTO_IP, IP_TTL, &tr->ttl, sizeof(tr->ttl));
	return (sock);
}
