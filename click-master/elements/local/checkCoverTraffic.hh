#ifndef CLICK_CHECKCOVERTRAFFIC_HH
#define CLICK_CHECKCOVERTRAFFIC_HH
#include <click/element.hh>
CLICK_DECLS

class checkCoverTraffic : public Element 
{ 
	public:
  		checkCoverTraffic();
  		~checkCoverTraffic();

  		const char *class_name() const	{ return "checkCoverTraffic"; }
  		const char *port_count() const	{ return "1/1"; }
  		const char *processing() const  { return PUSH; }

  		void push(int, Packet *);
  		//int configure(Vector<String>&, ErrorHandler*);

};


CLICK_ENDDECLS
#endif