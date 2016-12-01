#ifndef CLICK_CORRELATOR_HH
#define CLICK_CORRELATOR_HH
#include <click/element.hh>
CLICK_DECLS

/*
=c
Correlator()

PROBABILITY is probability packets sent to port 0 unmodified
-- express as int as a percentage (between 0 and 100).

=s local
Occasionally, injects a TCP RST.

=d
One input, two outputs.  With some probability p, send packets 
out of port 0 unmodified.  With  probability 1-p, drop
the received packet and generate a new packet addressed
to the sender, with the TCP RST flag set.

*/

class Correlator : public Element { 

private:
	String folder;
	char fileName[128];
	char fileOutput[128];

public:
	Correlator();
	~Correlator();

	const char *class_name() const { return "Correlator"; }
	const char *port_count() const { return "1/1"; }
	const char *processing() const  { return PUSH; }

	void push(int, Packet *);
	int configure(Vector<String>&, ErrorHandler*);

private:
	void snoopPacket(const struct click_ip*, const struct click_tcp*);

};


CLICK_ENDDECLS
#endif
