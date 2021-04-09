#include "traceroute.h"

void	catch_sigint(int signal)
{
	(void)signal;
	g_tr.state = 0;
}
