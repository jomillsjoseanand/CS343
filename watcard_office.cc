#include "soda.h"



// Constructor
WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers ) : 
    prt(prt), bank(bank), numCouriers(numCouriers) {
    // Create courier pool
    couriers = new Courier *[numCouriers];
    for (unsigned int i = 0; i < numCouriers; i++) {
        couriers.push_back(new Courier(prt, bank, *this, i));
    }
}

WATCard::FWATCard WATCardOffice::create(unsigned int sid, unsigned int amount) {
    Job *job = new Job(Args(sid, amount)); // Create a new job for creating a WATCard
    jobQueue.push(job);
    return job->result; // Return the future
}

WATCard::FWATCard WATCardOffice::transfer(unsigned int sid, unsigned int amount, WATCard *card) {
    Job *job = new Job(Args(sid, amount, card)); // Create a new job for transferring funds
    jobQueue.push(job);
    return job->result; // Return the future
}


// requestWork fetches the next Job from the WATCardOffice.
// - Each courier task calls requestWork, blocks until a Job request 
// is ready, and then receives the next Job request as the result
// of the call. 
// -  As soon as the request is satisfied (i.e., money is obtained from the bank), 
// the courier updates the student’s WATCard. 
// - There is a 1 in 6 chance, prng( 6 ) == 0, a courier loses a student’s WATCard 
// after the update. 
// - When the card is lost, the exception WATCardOffice::Lost is inserted into the future, 
// rather than making the future available, and the current WATCard is deleted
WATCardOffice::Job * WATCardOffice::requestWork() {
    // Wait for a job
    while (jobQueue.empty()) { 
        bench.wait();
    }    

    Job *job = jobQueue.front();
    jobQueue.pop();
    return job;   
}

void WATCardOffice::main() {
    for ( ;; ) {
        _Accept(~WATCardOffice) {
            break;
        } or _Accept(create) {
            bench.signalBlock();
        } or _Accept(transfer) {
            bench.signalBlock();
        } or _Accept(requestWork) {
        }
    }
}

WATCardOffice::~WATCardOffice() {
    for (unsigned int i = 0; i < numCouriers; i++) {
        delete couriers[i];
    }
    delete[] couriers;
}

// - Create method eventually polulates future
// - Job struct encapsualtes a request (request args + result stored in futrue which will eventuall be updated by create)
// - WatcardOffice produces withdraw requests to the which are delivered via courier

// - The watcard office is an admin task 
// -  transfer funds from their bank account to their WATCard to buy a soda.
// - Initially, the WATCard office creates a fixed-sized courier pool with numCouriers
// courier tasks to communicate with the bank. (Additional couriers may not be created after the WATCardOffice
// begins.) 

// - A student performs an asynchronous call to create to create a “real” WATCard with an initial balance.
// - A future WATCard is returned and sufficient funds are subsequently obtained from the bank (see Parent task) via
// a courier to satisfy the create request.
// - A student performs an asynchronous call to transfer when their WATCard
// indicates there is insufficient funds to buy a soda. 

// - The WATCard office is empowered to transfer funds
// from a student’s bank-account to its WATCard by sending a request 
// through a courier to the bank. 
// - Each courier task calls requestWork, blocks until a Job request 
// is ready, and then receives the next Job request as the result
// of the call. 
// -  As soon as the request is satisfied (i.e., money is obtained from the bank), 
// the courier updates the student’s WATCard. 
// - There is a 1 in 6 chance, prng( 6 ) == 0, a courier loses a student’s WATCard 
// after the update. 
// - When the card is lost, the exception WATCardOffice::Lost is inserted into the future, 
// rather than making the future available, and the current WATCard is deleted
