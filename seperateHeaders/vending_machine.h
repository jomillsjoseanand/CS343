
#ifndef __NAME_SERVER_H__
#define __NAME_SERVER_H__

_Task VendingMachine {
	void main();
    Printer & prt;
    NameServer & nameServer;
    unsigned int id; 
    unsigned int sodaCost;
	unsigned int inventory[4];
    bool restocking;                           // Flag to indicate if the machine is being restocked

    
  public:
	_Exception Funds {};					// insufficient funds
	_Exception Stock {};					// flavour out of stock
	_Exception Free {};						// free, advertisement
	
	VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost );
	void buy( BottlingPlant::Flavours flavour, WATCard & card );
	unsigned int * inventory() __attribute__(( warn_unused_result ));
	void restocked();
	_Nomutex unsigned int cost() const;
	_Nomutex unsigned int getId() const;
};

#endif