#include "traceroute.h"

void	free_args(t_tr *tr)
{
	free(tr->dest_name);
	free(tr->dest_ip);
	free(tr->pr.sacrecv);
}

void	init_tr(t_tr *tr)
{
	tr->h = 0;
	tr->df_bit = ntohs(IP_DF);
	tr->nqueries = 3;
	tr->tos = 0;
	tr->wait_time = 1;
	tr->ttl = 1;
	tr->max_ttl = DEF_TTL_MAX;
	tr->count_max = -1;
	tr->msg_sent = 0;
	tr->msg_recv_count = 0;
	tr->fqdn = NULL;
	tr->dest_name = NULL;
	tr->dest_ip = NULL;
	tr->pid = getpid();
	tr->datalen = TR_PKT_S;
	tr->pr.sasend = NULL;
	tr->pr.sacrecv = NULL;
	tr->pr.salen = 0;
}
