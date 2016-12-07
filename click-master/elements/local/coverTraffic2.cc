/*
 */

#include <click/config.h>
#include "coverTraffic2.hh"
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
#include <arpa/inet.h>

#define NUM_FLOWS 51

CLICK_DECLS

coverTraffic2::coverTraffic2(): _prob(0)
{
}

coverTraffic2::~coverTraffic2()
{
}

int coverTraffic2::configure(Vector<String> &conf, ErrorHandler *errh)
{
	srand(time(NULL)); //seed randomizer.
	int new_prob = 100;
	if (Args(conf, this, errh).read_p("PROBABILITY", new_prob).complete() < 0)
	{
		return -1;
	}
	if(new_prob < 0 || new_prob > 100)
	{
		return -1;
	}
	_prob = new_prob;
	
	flowArray = (long*)calloc(NUM_FLOWS, sizeof(long));
	
	return 0;
}


void coverTraffic2::push(int, Packet *p)
{
	int v1 = rand() % 100;
	
	ip = (struct click_ip *) p->ip_header();
	flowArray[int((ip->ip_dst.s_addr&0xFF000000)>>24)] += p->length();
	
	if (v1 < _prob)
	{
		//This makes a clone of packet P as it come in. 
		//Then we set the ttl of q to 1, send both traffics out.
		WritablePacket *q = p->uniqueify();
		ip = (struct click_ip *) q->ip_header();
		ip->ip_ttl = 1;
		
		min = flowArray[1];
		minFlow = 1;
		
		for (int i = 2; i <= NUM_FLOWS; i++)
		{
			if (flowArray[i] < minFlow)
			{
				min = flowArray[i];
				minFlow = i;
			}
		}
	
		sprintf(minAddress, "19.19.19.%d", minFlow);
	
		inet_aton(minAddress, &(ip->ip_dst));
		
		flowArray[minFlow] += q->length();

		output(0).push(q);
	}

	output(0).push(p);
}


CLICK_ENDDECLS
EXPORT_ELEMENT(coverTraffic2)
ELEMENT_MT_SAFE(coverTraffic2)
