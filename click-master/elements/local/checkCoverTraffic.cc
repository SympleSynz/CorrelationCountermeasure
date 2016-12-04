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

checkCoverTraffice::checkCoverTraffice(): _prob(0)
{
}

coverTraffic::~checkCoverTraffice()
{
}

int checkCoverTraffice::configure(Vector<String> &conf, ErrorHandler *errh)
{
    return 0;
}


void checkCoverTraffice::push(int, Packet *p)
{
	struct click_ip *ip;
	ip = (struct click_ip *) q->ip_header();
	if (ip->ip_ttl != 1)
		output(0).push(p);
	else
		p->kill();  
}


CLICK_ENDDECLS
EXPORT_ELEMENT(checkCoverTraffice)
ELEMENT_MT_SAFE(checkCoverTraffice)
