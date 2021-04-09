#include "traceroute.h"

void	free_args(t_tr *tr)
{
	free(tr->dest_name);
	free(tr->dest_ip);
	free(tr->pr.sacrecv);
}

void	init_tr(t_tr *tr)
{
	tr->v = 0;
	tr->h = 0;
	tr->q = 0;
	tr->d = 0;
	tr->ttl = 0;
	tr->count_max = -1;
	tr->rtt_min = -1;
	tr->rtt_max = -1;
	tr->rtt_sum = 0;
	tr->rtt_sum_sq = 0;
	tr->msg_sent = 0;
	tr->msg_count = 0;
	tr->msg_recv_count = 0;
	tr->fqdn = NULL;
	tr->dest_name = NULL;
	tr->dest_ip = NULL;
	tr->pid = getpid() & 0xffff;
	tr->datalen = PING_PKT_S;
	tr->pr.sasend = NULL;
	tr->pr.sacrecv = NULL;
	tr->pr.salen = 0;
}
