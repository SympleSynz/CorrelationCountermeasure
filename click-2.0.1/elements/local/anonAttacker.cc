/*
 */

#include <click/config.h>
#include "attacker.hh"
#include <clicknet/ip.h>
#include <clicknet/tcp.h>
#include <clicknet/ether.h>
#include <click/args.hh>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/router.hh>
#include <sys/stat.h>
CLICK_DECLS

AnonAttacker::AnonAttacker(int side)
{
	client = side;
	
	if (client)
	{
		folder = "client";
	} else
	{
		folder = "server";
	}
	
	struct stat st = {0};
	if (stat(folder, &st) == -1)
	{
		mkdir(folder, 0744);
	}
}

AnonAttacker::~AnonAttacker()
{

}

int AnonAttacker::configure(Vector<String> &conf, ErrorHandler *errh)
{
	int new_prob = 100;
	if (Args(conf, this, errh).read_p("PROBABILITY", new_prob).complete() < 0)
	{
		return -1;
	}

	if (new_prob < 0 || new_prob > 100) 
	{
		return -1;
	}
	_prob = new_prob;
	return 0;
}


void AnonAttacker::push(int, Packet *p)
{
	if ( p->has_network_header	() )
	{
		const struct click_ip* ipHeader = p->ip_header();
		
		if (ipHeader->ip_p == IP_PROTO_TCP )
		{
			const struct click_tcp* tcpHeader = p->tcp_header();
			snoopPacket(ipHeader, tcpHeader);
		}
	}
	
	output(0).push(p);
	
	
	
	
	int randomInt = (std::rand() % 100);
	
	if (randomInt < _prob)
	{
		output(0).push(p);
		// output on port 0 unmodified
	} else
	{
		p->kill();
		output(1).push(make_RST());
		// create a new packet with RST
		// discard the received packet
		// output new packet on port 1
	}
}

void AnonAttacker::snoopPacket(const struct click_ip* ipHeader, const struct click_tcp* tcpHeader)
{
	char* address;
	uint16_t port;
	
	if (clientSide)
	{
		address = ntoa(ipHeader->ip_dest);
		port = tcpHeader->th_dport;
	} else
	{
		address = ntoa(ipHeader->ip_src);
		port = tcpHeader->th_sport;
	}
	char* fileName;
	
	sprintf(fileName, "./%s/%s:%d.out", folder, address, port);
	
	File* output = fopen(fileName, "a");
	fprintf(output, "%d\t%d\n", time(0) ,ipHeader->ip_len );
	fclose(output);
}



CLICK_ENDDECLS
EXPORT_ELEMENT(AnonAttacker)
ELEMENT_MT_SAFE(AnonAttacker)
