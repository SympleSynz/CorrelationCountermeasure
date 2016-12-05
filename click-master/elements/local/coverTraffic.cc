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

CLICK_DECLS

coverTraffic::coverTraffic(): _prob(0)
{
}

coverTraffic::~coverTraffic()
{
}

int coverTraffic::configure(Vector<String> &conf, ErrorHandler *errh)
{
	srand(time(NULL)); //seed randomizer.
	int new_prob = 100;
	if (Args(conf, this, errh).read_p("PROBABILITY", new_prob).complete() < 0)
	{
		return -1;
	}
	if(new_prob < 0 || new_prob > 100)
	{
		return -1
	}
	_prob = new_prob;
	return 0;
}


void coverTraffic::push(int, Packet *p)
{
		int v1 = rand() % 100;
	if (v1 < _prob)
		output(0).push(p);
	else
	{
		struct click_ether *ether;
		struct click_ip *ip;
		struct click_tcp *tcp;
		struct click_ether *ether_recv;
		struct click_ip *ip_recv;
		struct click_tcp *tcp_recv;


		WritablePacket *q = Packet::make(p->data(), sizeof(*ether) + sizeof(*ip) + sizeof(*tcp) + p->length());

		ether = (struct click_ether *) q->data();
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
		memcpy((void *) &(ip->ip_src), (void *) &(ip_recv->ip_dst), sizeof(ip_recv->ip_dst));
		memcpy((void *) &(ip->ip_dst), (void *) &(ip_recv->ip_src), sizeof(ip_recv->ip_src));
		ip->ip_sum = click_in_cksum((unsigned char *)ip, sizeof(click_ip));

		// TCP fields
		memcpy((void *) &(tcp->th_sport), (void *) &(tcp_recv->th_dport), sizeof(tcp_recv->th_dport));
		memcpy((void *) &(tcp->th_dport), (void *) &(tcp_recv->th_sport), sizeof(tcp_recv->th_sport));
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

		// Packet q is ready

		output(0).push(q);
		output(0).push(p);
	}

  
}


CLICK_ENDDECLS
EXPORT_ELEMENT(coverTraffic)
ELEMENT_MT_SAFE(coverTraffic)
