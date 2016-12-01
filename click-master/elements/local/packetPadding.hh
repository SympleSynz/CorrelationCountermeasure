#ifndef CLICK_PACKETPADDING_HH
#define CLICK_PACKETPADDING_HH
#include <click/element.hh>
CLICK_DECLS

class packetPadding : public Element 
{ 
	public:

  		int _prob;

	public:
  		packetPadding();
  		~packetPadding();

  		const char *class_name() const	{ return "packetPadding"; }
  		const char *port_count() const	{ return "1/2"; }
  		const char *processing() const  { return PUSH; }

  		void push(int, Packet *);
  		int configure(Vector<String>&, ErrorHandler*);

};


CLICK_ENDDECLS
#endif
