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
		mkdir(folder.c_str(), 0777);
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
	snoopPacket(p);
	
	output(0).push(p);
}

void Correlator::snoopPacket(Packet *p)
{
	char srcAddress[16];
	char destAddress[16];
	uint16_t srcPort;
	uint16_t destPort;
	
	const struct click_ip* ipHeader = p->ip_header();
	const struct click_tcp* tcpHeader = p->tcp_header();
	
	//srcAddress = inet_ntoa(ipHeader->ip_src);
	//destAddress = inet_ntoa(ipHeader->ip_dst);
	
	clock_gettime(CLOCK_MONOTONIC, &time_struct);
	currentTime = ( time_struct.tv_sec * 1000000 ) + ( time_struct.tv_nsec / 1000 );
	
	sprintf(srcAddress, "%d.%d.%d.%d",
		int(ipHeader->ip_src.s_addr&0xFF), 
		int((ipHeader->ip_src.s_addr&0xFF00)>>8),
		int((ipHeader->ip_src.s_addr&0xFF0000)>>16),
		int((ipHeader->ip_src.s_addr&0xFF000000)>>24));

	sprintf(destAddress, "%d.%d.%d.%d",
		int(ipHeader->ip_dst.s_addr&0xFF), 
		int((ipHeader->ip_dst.s_addr&0xFF00)>>8),
		int((ipHeader->ip_dst.s_addr&0xFF0000)>>16),
		int((ipHeader->ip_dst.s_addr&0xFF000000)>>24));
	
	srcPort = tcpHeader->th_sport;
	destPort = tcpHeader->th_dport;
	
	sprintf(fileName, "./%s/%s-%d_%s-%d.out", folder.c_str(), srcAddress, srcPort, destAddress, destPort);
	
	FILE* output = fopen(fileName, "a");
	fprintf(output, "%ld\t%d\n", currentTime - startTime , p->length() );
	fclose(output);
}



CLICK_ENDDECLS
EXPORT_ELEMENT(Correlator)
ELEMENT_MT_SAFE(Correlator)
