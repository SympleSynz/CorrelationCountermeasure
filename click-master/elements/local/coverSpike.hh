#ifndef CLICK_COVERTRAFFIC_HH
#define CLICK_COVERTRAFFIC_HH
#include <click/element.hh>
CLICK_DECLS

typedef struct _flow
{
	char address[16];
	int active;
} Flow;

class CoverSpike : public Element 
{ 
	public:

  		int _prob;
		Flow* flowArray;

		int bigSpikes[3];
		int littleSpikes[3];

	public:
  		CoverTraffic();
  		~CoverTraffic();

  		const char *class_name() const	{ return "CoverSpike"; }
  		const char *port_count() const	{ return "1/1"; }
  		const char *processing() const  { return PUSH; }

  		void push(int, Packet *);
  		int configure(Vector<String>&, ErrorHandler*);
		void send_cover(Flow *, int, Packet *);

};


CLICK_ENDDECLS
#endif
