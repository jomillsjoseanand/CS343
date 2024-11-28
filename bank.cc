#include "bank.h"

Bank::Bank( unsigned int numStudents ) numStudents(numStudents), accounts(numStudents, 0) {
}

void Bank::deposit( unsigned int id, unsigned int amount ){
    accounts[id] += amount; // Add amount to the student's account
}

void Bank::withdraw( unsigned int id, unsigned int amount ){
    
    // The courier waits until enough money has been deposited, which 
    // may require multiple deposits.
    while (accounts[id] < amount) {
        _Accept(deposit){}
    }
    accounts[id] -= amount; // deduct requested amount 
}
