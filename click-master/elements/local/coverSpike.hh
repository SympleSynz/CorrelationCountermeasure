#ifndef CLICK_COVERSPIKE_HH
#define CLICK_COVERSPIKE_HH
#include <click/element.hh>
CLICK_DECLS

typedef struct _flow2
{
	char address[16];
	int active;
} Flow2;

class CoverSpike : public Element 
{ 
	public:

  		int _prob;
		Flow2* flowArray;

		int bigSpike[3];
		int littleSpike[3];

	public:
  		CoverSpike();
  		~CoverSpike();

  		const char *class_name() const	{ return "CoverSpike"; }
  		const char *port_count() const	{ return "1/1"; }
  		const char *processing() const  { return PUSH; }

  		void push(int, Packet *);
  		int configure(Vector<String>&, ErrorHandler*);
		void send_cover(Flow2 *, int, Packet *);

};


CLICK_ENDDECLS
#endif
