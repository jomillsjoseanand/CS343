#include "soda.h"

// - The WATCard manages the money associated with a card. 
// // -- When a WATCard is created, it has a $0 balance.
// - A courier calls deposit after a funds transfer
// - A vending machine calls withdraw when a soda is purchased. 
// - A student and a vending machine call getBalance to determine the balance.
// - FWATCard is a future pointer to a student’s WATCard for synchronizing access 
// to the WATCard between the student and the courier.

// Constructor
WATCard::WATCard() : balance(0) {}

// Destructor
WATCard::~WATCard() {}

// Deposit money into the WATCard
void WATCard::deposit(unsigned int amount) {
    mutex.P();
    balance += amount;
    mutex.V();
}

// Withdraw money from the WATCard
void WATCard::withdraw(unsigned int amount) {
    mutex.P();
    balance -= amount;
    mutex.V();
}

unsigned int WATCard::getBalance(){
    return balance;
}