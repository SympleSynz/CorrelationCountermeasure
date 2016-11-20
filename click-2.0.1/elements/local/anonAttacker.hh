#ifndef CLICK_ATTACKER_HH
#define CLICK_ATTACKER_HH
#include <click/element.hh>
CLICK_DECLS

/*
=c
Attacker()

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

class Attacker : public Element { 

private:
	char* folder;
	char fileName[64];
	char fileOutput[64];

public:
	Attacker();
	~Attacker();

	const char *class_name() const { return "TCPReseter"; }
	const char *port_count() const { return "1/2"; }
	const char *processing() cons  { return PUSH; }

	void push(int, Packet *);
	int configure(Vector<String>&, ErrorHandler*);

private:
	void Attacker::snoopPacket(const struct click_ip*, const struct click_tcp*);

};


CLICK_ENDDECLS
#endif
