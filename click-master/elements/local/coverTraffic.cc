/*
 */

#include <click/config.h>
#include "coverTraffic.hh"
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

CoverTraffic::CoverTraffic(): _prob(0)
{
}

CoverTraffic::~CoverTraffic()
{
}

int CoverTraffic::configure(Vector<String> &conf, ErrorHandler *errh)
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
	
	flowArray = (Flow*)calloc(NUM_FLOWS, sizeof(Flow));
	
	return 0;
}


/*void CoverTraffic::push(int, Packet *p)
{
	int v1 = rand() % 100;
	
	ip = (struct click_ip *) p->ip_header();
	flowArray[int((ip->ip_dst.s_addr&0xFF000000)>>24)] += p->length();
	
	if (v1 < _prob)
	{
		//This makes a clone of packet P as it come in. 
		//Then we set the ttl of q to 1, send both traffics out.
		Packet *q = p->clone();
		WritablePacket *r = q->uniqueify();
		
		ip = (struct click_ip *) r->ip_header();
		ip->ip_ttl = 1;
		
		min = flowArray[1];
		minFlow = 1;
		
		for (int i = 2; i < NUM_FLOWS; i++)
		{
			if (flowArray[i] < minFlow)
			{
				min = flowArray[i];
				minFlow = i;
			}
		}
	
		sprintf(minAddress, "19.19.19.%d", minFlow);
	
		inet_aton(minAddress, &(ip->ip_dst));
		
		flowArray[minFlow] += r->length();

		output(0).push(r);
	}
	
	//click_chatter("Got to output(0).push(p)");

	output(0).push(p);
}*/

void CoverTraffic::push(int, Packet *p)
{
	int v1 = rand() % 100;
	
	thisFlow = int((ip->ip_dst.s_addr&0xFF000000)>>24);
	
	ip = (struct click_ip *) p->ip_header();
	
	if ( flowArray[ thisFlow ].flowTraffic == 0)
	{
		sprintf(flowArray[ thisFlow ].address, "%d.%d.%d.%d",
			int(ip->ip_src.s_addr&0xFF), 
			int((ip->ip_src.s_addr&0xFF00)>>8),
			int((ip->ip_src.s_addr&0xFF0000)>>16),
			int((ip->ip_src.s_addr&0xFF000000)>>24));
	}
	flowArray[thisFlow].flowTraffic += p->length();
	
	if (v1 < _prob)
	{
		struct click_ether *ether;
		struct click_tcp *tcp;
		struct click_ether *ether_recv;
		struct click_ip *ip_recv;
		struct click_tcp *tcp_recv;

		WritablePacket *q = Packet::make(p->data(), sizeof(*ether) + sizeof(*ip) + sizeof(*tcp) + p->length());

		ether = (struct click_ether *) p->data();
		q->set_ether_header(ether);
		ip = (struct click_ip *) q->ip_header();
		q->set_ip_header(ip, sizeof(click_ip));
		tcp = (struct click_tcp *) q->tcp_header(); // (ip + 1);
		//q->set_ip_header(ip, sizeof(click_ip));
		
		ether_recv = (struct click_ether *) p->ether_header();
		ip_recv = (struct click_ip *) p->ip_header();
		tcp_recv = (struct click_tcp *) p->tcp_header();  //(ip_recv + 1);

		// ETHER fields
		ether->ether_type = ether_recv->ether_type;
		memcpy(ether->ether_dhost, ether_recv->ether_shost, 6);
		memcpy(ether->ether_shost, ether_recv->ether_dhost, 6);

		// IP fields
		ip->ip_v = 4;
		ip->ip_hl = 5;
		ip->ip_tos = 1;
		ip->ip_len = htons(q->length()-14);
		ip->ip_id = htons(0);
		ip->ip_off = htons(IP_DF);
		ip->ip_ttl = 1;
		ip->ip_p = IP_PROTO_TCP;
		ip->ip_sum = 0;
		memcpy((void *) &(ip->ip_src), (void *) &(ip_recv->ip_src), sizeof(ip_recv->ip_src));
		memcpy((void *) &(ip->ip_dst), (void *) &(ip_recv->ip_dst), sizeof(ip_recv->ip_dst));
		ip->ip_sum = click_in_cksum((unsigned char *)ip, sizeof(click_ip));

		// TCP fields
		memcpy((void *) &(tcp->th_sport), (void *) &(tcp_recv->th_sport), sizeof(tcp_recv->th_sport));
		memcpy((void *) &(tcp->th_dport), (void *) &(tcp_recv->th_dport), sizeof(tcp_recv->th_dport));
		tcp->th_seq = tcp_recv->th_seq;
		tcp->th_ack = tcp_recv->th_ack;
		tcp->th_off = 5;
		tcp->th_flags = 0x4; // RST bit set
		tcp->th_win = htons(32120);
		tcp->th_sum = htons(0);
		tcp->th_urp = htons(0);

		// now calculate tcp header cksum
		unsigned csum = click_in_cksum((unsigned char *)tcp, sizeof(click_tcp));
		tcp->th_sum = click_in_cksum_pseudohdr(csum, ip, sizeof(click_tcp));
		
		min = flowArray[thisFlow].flowTraffic;
		minFlow = thisFlow;
		
		for (int i = 1; i < NUM_FLOWS; i++)
		{
			if ((flowArray[i].flowTraffic > 0) && (flowArray[i].flowTraffic < minFlow))
			{
				min = flowArray[i].flowTraffic;
				minFlow = i;
			}
		}
		
		sprintf(minAddress, "19.19.19.%d", minFlow);
		
		inet_aton(minAddress, &(ip->ip_dst));
		inet_aton(flowArray[minFlow].address, &(ip->ip_src) );
		
		flowArray[minFlow].flowTraffic += q->length();

		// Packet q is ready

		output(0).push(q);
	}
	
	output(0).push(p);
  
}


CLICK_ENDDECLS
EXPORT_ELEMENT(CoverTraffic)
ELEMENT_MT_SAFE(CoverTraffic)
