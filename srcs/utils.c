#include "traceroute.h"

unsigned short	checksum(void *b, int len)
{
	unsigned short	*buf;
	unsigned int	sum;
	unsigned short	result;

	buf = b;
	sum = 0;
	while (len > 1)
	{
		sum += *buf++;
		len -= 2;
	}
	if (len == 1)
		sum += *(unsigned char *)buf;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	result = ~sum;
	return (result);
}

void	print_usage(void)
{
	dprintf(2, "usage: ft_traceroute [-hF] "
		"[-f first ttl] [-m max ttl] [-q nqueries] host [packet len]\n");
}

int		return_error(char *err, int val)
{
	dprintf(2, "%s", err);
	return (val);
}
