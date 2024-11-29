#ifndef __GROUPOFF_H__
#define __GROUPOFF_H__

_Task Groupoff {
	void main();
    Printer & prt;
    unsigned int numStudents;
    unsigned int sodaCost;
    unsigned int groupoffDelay;
    vector<WATCard::FWATCard> giftCards;
  public:
	Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
	WATCard::FWATCard giftCard( unsigned int id );
};

#endif