#ifndef __STUDENT_H__
#define __STUDENT_H__

_Task Student {
	void main();
    Printer & prt;
    NameServer & nameServer;
    WATCardOffice & cardOffice;
    Groupoff & groupoff;
	unsigned int id;
    unsigned int maxPurchases;
    VendingMachine * vendingMachine;
    WATCard::FWATCard watCard;
    WATCard::FWATCard giftCard;
    unsigned int numPurchases;
    BottlingPlant::Flavour favouriteFlavour;
    bool lostCard;

  public:
	Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff,
			 unsigned int id, unsigned int maxPurchases );
};


#endif