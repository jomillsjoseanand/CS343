#ifndef __SODA_H__
#define __SODA_H__

#include "uPRNG.h"                  // For pseudorandom number generation
#include <uFuture.h>
#include <uSemaphore.h>

// Forward declare
_Task Groupoff;
_Task Student;
_Task WATCardOffice;
class WATCard;
_Task VendingMachine;
_Task NameServer;
_Task Truck;
_Task BottlingPlant;
_Monitor Bank;
_Task Parent;
_Monitor Printer;

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

_Task WATCardOffice {
	struct Args {
        unsigned int sid;       // Student ID
        unsigned int amount;    // Amount to transfer
        WATCard *card;          // Pointer to WATCard (nullable for create)
        Args(unsigned int sid, unsigned int amount, WATCard *card = nullptr)
            : sid(sid), amount(amount), card(card) {}
    };
    
    
    struct Job {							// marshalled arguments and return future
		Args args;							// call arguments (YOU DEFINE "Args")
		WATCard::FWATCard result;			// return future
		Job( Args args ) : args( args ) {}
	};
	_Task Courier { 

        Printer &prt;
        Bank &bank;
        WATCardOffice &office;
        unsigned int id;

        public:
            Courier(Printer &prt, Bank &bank, WATCardOffice &office, unsigned int id)
                : prt(prt), bank(bank), office(office), id(id) {}
    };					// communicates with bank


    Printer & prt;
    Bank & bank;
    unsigned int numCouriers;
    Courier **couriers;
    std::queue<Job *> jobQueue;             // Queue of job requests
    uCondition bench;

	void main();
  public:
	_Exception Lost {};						// lost WATCard
	WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers );
	~WATCardOffice();
    WATCard::FWATCard create( unsigned int sid, unsigned int amount ) __attribute__(( warn_unused_result ));
	WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard * card ) __attribute__(( warn_unused_result ));
	Job * requestWork() __attribute__(( warn_unused_result ));
};

class WATCard {
	WATCard( const WATCard & ) = delete;	// prevent copying
	WATCard( const WATCard && ) = delete;
	WATCard & operator=( const WATCard & ) = delete;
	WATCard & operator=( const WATCard && ) = delete;
    unsigned int balance;
    uSemaphore mutex;
    
  
  public:
	typedef Future_ISM<WATCard *> FWATCard;	// future watcard pointer
	WATCard();
	void deposit( unsigned int amount );
	void withdraw( unsigned int amount );
	unsigned int getBalance();
};

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

_Task NameServer {
	void main();
    Printer & prt;
    unsigned int numVendingMachines;
    unsigned int numStudents;
	VendingMachine ** vendingMachines;
    unsigned int * studentMachineIndex;
    uCondition bench; 

  public:
	NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents );
	~NameServer();
    void VMregister( VendingMachine * vendingmachine );
	VendingMachine * getMachine( unsigned int id ) __attribute__(( warn_unused_result ));
	VendingMachine ** getMachineList() __attribute__(( warn_unused_result ));
};

_Task Truck {
	void main();
    Printer & prt;
    NameServer & nameServer;
    BottlingPlant & plant;
	unsigned int numVendingMachines;
    unsigned int maxStockPerFlavour;
    unsigned int cargo[4];


  public:
	Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant,
		   unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
};

_Task BottlingPlant {
	void main();
    Printer & prt;
    NameServer & nameServer;
    unsigned int numVendingMachines;
	unsigned int maxShippedPerFlavour;
    unsigned int maxStockPerFlavour;
	unsigned int timeBetweenShipments;
    unsigned int inventory[4];
    Truck truck;
  
  public:
	enum Flavours { Blues_BlackCherry, Classic_CreamSoda, Rock_RootBeer, Jazz_Lime  };	// flavours of soda (YOU DEFINE)
	
    _Exception Shutdown {};					// shutdown plant
	
    BottlingPlant( Printer & prt, NameServer & nameServer, unsigned int numVendingMachines,
				 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
				 unsigned int timeBetweenShipments );
	void getShipment( unsigned int cargo[] );
};

_Monitor Bank {
    unsigned int numStudents;
    vector<unsigned int> accounts; // Store balances for each student
    vector<uCondition> sufficientFunds; // Conditions for waiting on sufficient funds

  public:
	Bank( unsigned int numStudents );
	void deposit( unsigned int id, unsigned int amount );
	void withdraw( unsigned int id, unsigned int amount );
};

_Task Parent {

    void main();
    Printer& prt;
    Bank& bank;
    unsigned int numStudents;
    unsigned int parentalDelay;
    
    public:
        Parent( Printer & prt, Bank & bank, unsigned int numStudents, unsigned int parentalDelay );
};

_Monitor Printer {
    enum Kind { Parent, Groupoff, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };

    unsigned int numStudents;
    unsigned int numVendingMachines;
    unsigned int numCouriers;
    unsigned int numColumns;           // Total number of columns (fixed + variable instances)


    struct BufferEntry {                    // Buffer to temporarily store vote and state informationn
        char state;                         // State to print
        bool empty = true;                  // Whether this buffer slot is empty
        int value1 = -1; 
        int value2 = -1;
    };

    vector<Buffer> buffer;           // Vector of buffers for storing each voter's state

    void flush();                    // Function to output and clear buffer content
    unsigned int determineColumn(Kind kind, unsigned int lid = 0) const;

    public:
        Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
        ~Printer();
        void print( Kind kind, char state );
        void print( Kind kind, char state, unsigned int value1 );
        void print( Kind kind, char state, unsigned int value1, unsigned int value2 );
        void print( Kind kind, unsigned int lid, char state );
        void print( Kind kind, unsigned int lid, char state, unsigned int value1 );
        void print( Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2 );
};

#endif