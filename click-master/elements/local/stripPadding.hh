#ifndef CLICK_STRIPPADDING_HH
#define CLICK_STRIPTPADDING_HH
#include <click/element.hh>
CLICK_DECLS

class stripPadding : public Element 
{ 
	public:

  		int _prob;

	public:
  		stripPadding();
  		~stripPadding();

  		const char *class_name() const	{ return "stripPadding"; }
  		const char *port_count() const	{ return "1/1"; }
  		const char *processing() const  { return PUSH; }

  		void push(int, Packet *);
  		int configure(Vector<String>&, ErrorHandler*);

};


CLICK_ENDDECLS
#endif
