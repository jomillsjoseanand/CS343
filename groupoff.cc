#include "soda.h"

// Constructor
Groupoff::Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay ) : 
    prt(prt), numStudents(numStudents), sodaCost(sodaCost), groupoffDelay(groupoffDelay) {
    prt.print(Printer::Kind::Groupoff, 'S');
    
    giftCards = vector<WATCard::FWATCard>(numStudents);
}

// giftCard
WATCard::FWATCard Groupoff::giftCard(unsigned int id) {
    return giftCards[id];
}

// Destructor
Groupoff::~Groupoff() {

    // Delete all watcard ptrs
    for (unsigned int i = 0; i < cards.size(); i++) {
        delete cards[i];
    }
}


// """
// - The Groupoff task begins by first accepting a call from all students to obtain a future gift-card. 

// - After which groupoff periodically creates and puts a real WATCard with value $SodaCost into a random future gift-card.
// - A future gift-card is assigned only once. 
// - Before each future gift-card is assigned a real WATCard, groupoff yields 
// for groupoffDelay times (not random). 
// - The groupoff loops until all the future gift-cards are assigned a real
// WATCard or a call to its destructor occurs. 

// - Since it must not block on the destructor call, it is necessary to use
// a terminating _Else on the accept statement. 
// - This use of _Else is not busy waiting because there are a finite
// number of students.
// - Note, the lifetime of the gift cards is the lifetime of all students, not the lifetime of the
// Groupoff thread.
// """

// Main function
void Groupoff::main() {
    unsigned int assignedCards = 0;
    while (assignedCards < numStudents) {
        _Accept(~Groupoff) {
            break;
        } or _Accept(giftCard) {
            assignedCards++;
        }
    }

    unsigned int assignedGiftCards[numStudents];
    for (unsigned int i = 0; i < numStudents; i++) {
        assignedGiftCards[i] = false;
    }

    // Assigning gift cards at random
    unsigned int assigned = 0;
    while (assigned < numStudents) {
        _Accept(~Groupoff) {
            break;
        } _Else {
            yield(groupoffDelay);
            unsigned int student = prng(numStudents);
            if (!assignedGiftCards[student]) {
                
                WATCard *giftCardObj = new WATCard();
                cards.push_back(giftCardObj);
                
                giftCardObj->deposit(sodaCost);

                giftCards[student].delivery(giftCardObj);
                
                // Print the assigned gift card
                prt.print(Printer::Kind::Groupoff, 'D', student, sodaCost);
                assigned++;
                assignedGiftCards[student] = true;
            }
        }
    }

    prt.print(Printer::Kind::Groupoff, 'F');
    _Accept(~Groupoff) {
    }
}
