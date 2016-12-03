#ifndef CLICK_COVERTRAFFIC_HH
#define CLICK_COVERTRAFFIC_HH
#include <click/element.hh>
CLICK_DECLS

class coverTraffic : public Element 
{ 
	public:

  		int _maxPad;

	public:
  		coverTraffic();
  		~coverTraffic();

  		const char *class_name() const	{ return "coverTraffic"; }
  		const char *port_count() const	{ return "1/1"; }
  		const char *processing() const  { return PUSH; }

  		void push(int, Packet *);
  		int configure(Vector<String>&, ErrorHandler*);

};


CLICK_ENDDECLS
#endif
