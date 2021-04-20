#include "traceroute.h"

t_tr	g_tr;

int	ret_val(int ret)
{
	if (ret < 0)
		return (ret);
	if (g_tr.msg_recv_count)
		return (0);
	else if (g_tr.msg_sent)
		return (2);
	return (1);
}

int	main(int argc, char **argv)
{
	int	ret;

	ret = 0;
	if (getuid() != 0)
	{
		printf("ft_traceroute: need root privilege\n");
		return (-1);
	}
	init_tr(&g_tr);
	if (get_args(&g_tr, argc, argv))
	{
		print_usage();
		free_args(&g_tr);
		return (-1);
	}
	if (g_tr.h)
		print_usage();
	else
		ret = traceroute(&g_tr);
	free_args(&g_tr);
	return (ret_val(ret));
}
