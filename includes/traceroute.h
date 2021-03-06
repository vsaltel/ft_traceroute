#ifndef TRACEROUTE_H
# define TRACEROUTE_H

# define BUFSIZE 1500
# define TR_PKT_S 60
# define IP_STR_SIZE 129
# define DEF_TTL_MAX 30
# define RECV_TIMEOUT 1

# include <stdio.h>
# include <signal.h>
# include <netdb.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netinet/in_systm.h>
# include <netinet/in.h>
# include <netinet/ip.h>
# include <netinet/ip_icmp.h>
# include <sys/time.h>

# include "libft.h"

typedef struct s_recv_pckt
{
	struct iphdr	ip;
	struct icmphdr	hdr;
	char			msg[BUFSIZE];
}				t_recv_pckt;

typedef struct s_send_pckt
{
	struct ip		ip;
	struct icmp		icmp;
	char			msg[BUFSIZE];
}				t_send_pckt;

typedef struct s_proto
{
	struct sockaddr	*sasend;
	struct sockaddr	*sacrecv;
	socklen_t		salen;
}				t_proto;

typedef struct s_tr
{
	int				state;
	int				h;
	int				df_bit;
	int				tos;
	unsigned int	nqueries;
	unsigned int	wait_time;
	int				ttl;
	int				max_ttl;
	int				count_max;
	struct timeval	bef;
	struct timeval	aft;
	int				msg_sent;
	int				msg_recv_count;
	char			*fqdn;
	char			*dest_name;
	char			*dest_ip;
	int				datalen;
	int				sockfd;
	t_proto			pr;
	pid_t			pid;
}				t_tr;

extern t_tr	g_tr;

/*
** srcs/args.c
*/

int				get_args(t_tr *tr, int ac, char **av);

/*
** srcs/args_utils.c
*/

void			init_tr(t_tr *tr);
void			free_args(t_tr *tr);

/*
** srcs/conv_addr.c
*/

char			*set_inetaddr(struct sockaddr *sa);

/*
** srcs/traceroute.c
*/

int				traceroute(t_tr *tr);

/*
** srcs/recv_msg.c
*/

void			recv_msg(t_tr *tr, t_recv_pckt *pckt, char **last_ip);

/*
** srcs/rev_dns.c
*/

struct addrinfo	*reverse_dns_info(char *host, char *serv,
					int family, int socktype);
char			*get_fqdn_info(struct sockaddr *addr);

/*
** srcs/send_msg.c
*/

void			set_send_pckt(t_send_pckt *pckt);
void			send_msg(void *pckt);

/*
** srcs/socket.c
*/

int				set_socket(t_tr *tr);

/*
** srcs/signal.c
*/

void			catch_sigint(int signal);

/*
** srcs/utils.c
*/

unsigned short	checksum(void *b, int len);
void			print_usage(void);
int				return_error(char *err, int val);

#endif
