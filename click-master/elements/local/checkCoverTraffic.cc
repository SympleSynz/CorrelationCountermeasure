/*
 */

#include <click/config.h>
#include "checkCoverTraffic.hh"
#include <clicknet/ip.h>
#include <clicknet/tcp.h>
#include <clicknet/ether.h>
#include <click/args.hh>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/router.hh>

#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

CLICK_DECLS

checkCoverTraffic::checkCoverTraffic()
{
}

checkCoverTraffic::~checkCoverTraffic()
{
}

/*int checkCoverTraffic::configure(Vector<String> &conf, ErrorHandler *errh)
{
    return 0;
}
*/

void checkCoverTraffic::push(int, Packet *p)
{
	struct click_ip *ip;
	ip = (struct click_ip *) p->ip_header();
	if (ip->ip_ttl != 1)
	{
		output(0).push(p);
		//click_chatter("Push Packet: ttl=%d", ip->ip_ttl);
	}
	else
	{
		p->kill();  
		//click_chatter("Kill Packet: ttl=%d", ip->ip_ttl);
	}
}


CLICK_ENDDECLS
EXPORT_ELEMENT(checkCoverTraffic)
ELEMENT_MT_SAFE(checkCoverTraffic)
