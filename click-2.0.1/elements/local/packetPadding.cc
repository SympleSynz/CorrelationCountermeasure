/*
 */

#include <click/config.h>
#include "packetPadding.hh"
#include <clicknet/ip.h>
#include <clicknet/tcp.h>
#include <clicknet/ether.h>
#include <click/args.hh>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/router.hh>
CLICK_DECLS

packetPadding::packetPadding(): _prob(0)
{
}

packetPadding::~packetPadding()
{
}

int packetPadding::configure(Vector<String> &conf, ErrorHandler *errh)
{
    int new_prob = 100;
    if (Args(conf, this, errh).read_p("PROBABILITY", new_prob).complete() < 0)
        return -1;

    if (new_prob < 0 || new_prob > 100) {
        return -1;
    }
    _prob = new_prob;
    return 0;
}


void packetPadding::push(int, Packet *p)
{
	struct click_ether *ether;
	struct click_ip *ip;
	struct click_tcp *tcp;
	struct click_ether *ether_recv;
	struct click_ip *ip_recv;
	struct click_tcp *tcp_recv;

	//This creates an empty packet that we can create
	WritablePacket *q = Packet::make(sizeof(*ether) + sizeof(*ip) + sizeof(*tcp));
	click_chatter("Make Packet: %d, %d, %d, %d", q->length(), sizeof(*ether), sizeof(*ip),  sizeof(*tcp));
	if (q == 0) 
	{
		click_chatter("in packetPadding: cannot make packet!");
		assert(0);
	}
	//Create a char* that will have a padding of "0" for a random value 0-999
	int paddingBytes = rand() % 1000;
	char* padding;
	memset(padding, '0', 1);
	for (int i = 0; i < (paddingBytes - 2); ++i)
	{
		memcpy((padding + padding.length()), "0", 1);
	}
	//add a termination character at the end to identify the length of padding later
	memcpy((padding + padding->length()), "\0", 1);
	//add the original data from the original packet
	memcpy((padding + padding->length()), p->data(), p->length());
	//Put all that in the new packet
	memset(q->data(), padding, padding->length());

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
	ip->ip_ttl = 255;
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

	p->kill();
	output(1).push(q);

}


CLICK_ENDDECLS
EXPORT_ELEMENT(packetPadding)
ELEMENT_MT_SAFE(packetPadding)
