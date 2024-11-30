#include "soda.h"

Bank::Bank( unsigned int numStudents ) : numStudents(numStudents), accounts(numStudents, 0), sufficientFunds(numStudents) {
    
}

void Bank::deposit( unsigned int id, unsigned int amount ){
    accounts[id] += amount; // Add amount to the student's account
    sufficientFunds[id].signal(); // Wake up any couriers waiting for funds
}

void Bank::withdraw( unsigned int id, unsigned int amount ){
    
    // The courier waits until enough money has been deposited, which 
    // may require multiple deposits.
    while (accounts[id] < amount) {
        sufficientFunds[id].wait(); // Wait for enough funds to be deposited
    }
    accounts[id] -= amount; // deduct requested amount 
}

Bank::~Bank() {

}