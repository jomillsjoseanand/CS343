#ifndef __WATCARD_OFFICE_H__
#define __WATCARD_OFFICE_H__

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

#endif