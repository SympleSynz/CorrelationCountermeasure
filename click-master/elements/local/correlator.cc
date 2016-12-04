/*
 */

#include <click/config.h>
#include "correlator.hh"
#include <clicknet/ip.h>
#include <clicknet/tcp.h>
#include <clicknet/ether.h>
#include <click/args.hh>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/router.hh>
#include <string>
#include <sys/stat.h>
#include <arpa/inet.h>

CLICK_DECLS

Correlator::Correlator()
{
}

Correlator::~Correlator()
{

}

int Correlator::configure(Vector<String> &conf, ErrorHandler *errh)
{
	if (Args(conf, this, errh).read_p("FOLDER", folder).complete() < 0)
	{
		return -1;
	}
	
	struct stat st;
	if (stat(folder.c_str(), &st) == -1)
	{
		mkdir(folder.c_str(), 644);
		if (stat(folder.c_str(), &st) == -1)
		{
			return -1;
		}
	}
	
	clock_gettime(CLOCK_MONOTONIC, &time_struct);
	startTime = ( time_struct.tv_sec * 1000000 ) + ( time_struct.tv_nsec / 1000 );

	return 0;
}


void Correlator::push(int, Packet *p)
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
}

void Correlator::snoopPacket(const struct click_ip* ipHeader, const struct click_tcp* tcpHeader)
{
	char srcAddress[16];
	char destAddress[16];
	uint16_t srcPort;
	uint16_t destPort;
	
	//srcAddress = inet_ntoa(ipHeader->ip_src);
	//destAddress = inet_ntoa(ipHeader->ip_dst);
	
	clock_gettime(CLOCK_MONOTONIC, &time_struct);
	currentTime = ( time_struct.tv_sec * 1000000 ) + ( time_struct.tv_nsec / 1000 );
	
	sprintf(srcAddress, "%d.%d.%d.%d\n",
		int(ipHeader->ip_src.s_addr&0xFF), 
		int((ipHeader->ip_src.s_addr&0xFF00)>>8),
		int((ipHeader->ip_src.s_addr&0xFF0000)>>16),
		int((ipHeader->ip_src.s_addr&0xFF000000)>>24));

	sprintf(destAddress, "%d.%d.%d.%d\n",
		int(ipHeader->ip_dst.s_addr&0xFF), 
		int((ipHeader->ip_dst.s_addr&0xFF00)>>8),
		int((ipHeader->ip_dst.s_addr&0xFF0000)>>16),
		int((ipHeader->ip_dst.s_addr&0xFF000000)>>24));
	
	srcPort = tcpHeader->th_sport;
	destPort = tcpHeader->th_dport;
	
	sprintf(fileName, "./%s/%s-%d_%s-%d.out", folder.c_str(), srcAddress, srcPort, destAddress, destPort);
	
	FILE* output = fopen(fileName, "a");
	fprintf(output, "%ld\t%d\n", time(0) ,ipHeader->ip_len );
	fclose(output);
}



CLICK_ENDDECLS
EXPORT_ELEMENT(Correlator)
ELEMENT_MT_SAFE(Correlator)
