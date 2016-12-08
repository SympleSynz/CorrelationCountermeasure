#ifndef CLICK_COVERSPIKE_HH
#define CLICK_COVERSPIKE_HH
#include <click/element.hh>
CLICK_DECLS

#define NUM_FLOWS 51
#define NUM_SPIKE 5

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

		int bigSpike[NUM_SPIKE];
		int midSpike[NUM_SPIKE];
		int littleSpike[NUM_SPIKE];

	public:
  		CoverSpike();
  		~CoverSpike();

  		const char *class_name() const	{ return "CoverSpike"; }
  		const char *port_count() const	{ return "1/1"; }
  		const char *processing() const  { return PUSH; }

  		void push(int, Packet *);
  		int configure(Vector<String>&, ErrorHandler*);
		void send_cover(int, Packet *);

};


CLICK_ENDDECLS
#endif
