#ifndef CLICK_COVERTRAFFIC2_HH
#define CLICK_COVERTRAFFIC2_HH
#include <click/element.hh>
CLICK_DECLS

class coverTraffic2 : public Element 
{ 
	public:

  		int _prob;
		long* flowArray;

		long min;
		int minFlow;
		char minAdress[16];

		struct click_ip *ip;

	public:
  		coverTraffic2();
  		~coverTraffic2();

  		const char *class_name() const	{ return "coverTraffic"; }
  		const char *port_count() const	{ return "1/1"; }
  		const char *processing() const  { return PUSH; }

  		void push(int, Packet *);
  		int configure(Vector<String>&, ErrorHandler*);

};


CLICK_ENDDECLS
#endif
