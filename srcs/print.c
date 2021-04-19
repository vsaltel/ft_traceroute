#include "traceroute.h"

static double	square_root(double a)
{
	double	s;
	double	l;
	double	mid;

	s = 1;
	l = a;
	mid = 0;
	while (s <= l)
	{
		mid = (s + l) / 2;
		if ((mid * mid) == a)
			break ;
		if ((mid * mid) < a)
			s = mid + 1;
		else
			l = mid - 1;
	}
	mid = (s + l) / 2;
	return (mid);
}

void	print_final_stats(t_tr *tr)
{
	struct timeval	end_time;
	long			total_time;
	double			rtt_mdev;
	double			mean;
	double			smean;

	gettimeofday(&end_time, NULL);
	total_time = end_time.tv_sec * 1000 + end_time.tv_usec / 1000;
	total_time = total_time - \
		(tr->launch_time.tv_sec * 1000 + tr->launch_time.tv_usec / 1000);
	if (tr->q)
		printf("\n");
	printf("--- %s tr statistics ---\n", tr->dest_name);
	printf("%d packets transmitted, %d received, "
		"\%d%% packet loss, time: %ld ms\n",
		tr->msg_count, tr->msg_recv_count,
		((tr->msg_count - tr->msg_recv_count) / tr->msg_count) * 100,
		total_time);
	if (tr->msg_recv_count)
	{
		mean = tr->rtt_sum / tr->msg_recv_count;
		smean = tr->rtt_sum_sq / tr->msg_recv_count;
		rtt_mdev = square_root(smean - (mean * mean));
		printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n",
			tr->rtt_min, mean, tr->rtt_max, rtt_mdev);
	}
}
