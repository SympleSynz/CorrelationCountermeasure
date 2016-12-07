#ifndef CLICK_COVERTRAFFICAGAIN_HH
#define CLICK_COVERTRAFFICAGAIN_HH
#include <click/element.hh>
CLICK_DECLS

class CoverTrafficAgain : public Element 
{ 
	public:

  		int _prob;
		long* flowArray;

		long min;
		int minFlow;
		char minAddress[16];

		struct click_ip *ip;

	public:
  		CoverTrafficAgain();
  		~CoverTrafficAgain();

  		const char *class_name() const	{ return "coverTraffic"; }
  		const char *port_count() const	{ return "1/1"; }
  		const char *processing() const  { return PUSH; }

  		void push(int, Packet *);
  		int configure(Vector<String>&, ErrorHandler*);

};


CLICK_ENDDECLS
#endif
