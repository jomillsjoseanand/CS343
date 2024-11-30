#include "soda.h"

// Main courier task
void WATCardOffice::Courier::main() {
    // Print the courier's ID
    prt.print(Printer::Courier, id, 'S');

    for (;;) {
        _Accept( ~Courier ) {
            // Print the courier's ID
            prt.print(Printer::Courier, id, 'F');
            break;
        } 
        _Else {

            // Get the next job request
            Job *job = office.requestWork();
            
            if (job == nullptr) {

                break;
            }
            // Get the student's card -- this is a placeholder
            WATCard *card = job->args.card;
            if (card == nullptr) {
                card = new WATCard();
            }

            // Get the amount of money to transfer
            unsigned int amount = job->args.amount;
            
            // Get the student's bank account
            unsigned int sid = job->args.sid;
            
            // Get the bank's response
            prt.print(Printer::Courier, id, 't', sid, amount);
            bank.withdraw(sid, amount);

            card->deposit(amount);

            // 1 in 6 chance of losing the card
            // When the card is lost, the exception WATCardOffice::Lost is inserted into the future, 
            // rather than making the future available, and the current WATCard is deleted
            if (prng(6) == 0) {

                // Insert the exception into the future
                job->result.delivery(new WATCardOffice::Lost);


                // Print the lost card
                prt.print(Printer::Courier, id, 'L', sid);
                
                // Delete the card
                delete card;
            } else {
                // Print the transfer
                prt.print(Printer::Courier, id, 'T', sid, amount);

                
                // Deliver the card
                job->result.delivery(card);

            }

            delete job;
        }
    }

    _Accept( ~Courier ) {
        prt.print(Printer::Courier, id, 'F');
    }
}

// // Destructor for the courier
WATCardOffice::Courier::~Courier() {
}



// - The WATCard office is empowered to transfer funds
// from a student’s bank-account to its WATCard by sending a request 
// through a courier to the bank. 
// - Each courier task calls requestWork, blocks until a Job request 
// is ready, and then receives the next Job request as the result
// of the call. 
// -  As soon as the request is satisfied (i.e., money is obtained from the bank), 
// the courier updates the student’s WATCard. 

// // - There is a 1 in 6 chance, prng( 6 ) == 0, a courier loses a student’s WATCard 
// // after the update. 
// // - When the card is lost, the exception WATCardOffice::Lost is inserted into the future, 
// // rather than making the future available, and the current WATCard is deleted

