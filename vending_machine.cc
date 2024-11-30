#include "soda.h"
#include <iostream>

VendingMachine::VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost ) : prt(prt), nameServer(nameServer), id(id), sodaCost(sodaCost), restocking(false){
    for (int i = 0; i < 4; i++) {
        curr_inventory[i] = 0;
    }

    prt.print(Printer::Vending, id, 'S', sodaCost);

    // TODO: remove this 
    nameServer.VMregister(this);
}

VendingMachine::~VendingMachine() {
    // delete curr_inventory;
    prt.print(Printer::Vending, id, 'F');
}

_Nomutex unsigned int VendingMachine::cost() const {
    return sodaCost;
}

_Nomutex unsigned int VendingMachine::getId() const {
    return id;
}

unsigned int * VendingMachine::inventory() {
    // if (restocking) {
    //     bench.wait();
    // }
    restocking = true;
    prt.print(Printer::Vending, id, 'r');
    return curr_inventory;
}

void VendingMachine::restocked() {
    restocking = false;
    prt.print(Printer::Vending, id, 'R');
    // if (!bench.empty()) {
    //     bench.signalBlock();
    // }
}

void VendingMachine::buy( BottlingPlant::Flavours flavour, WATCard &card ) {
    if (restocking) return; // Ignore buy calls during restocking

    // vending machine first checks if the student has sufficient funds to purchase the soda 
    if (card.getBalance() < sodaCost) _Throw Funds(); // Check if the student has enough funds
    if (curr_inventory[flavour] == 0) _Throw Stock(); // Check if the soda is in stock

    // 1 in 5 chance the soda is free
    if (prng(4) == 0) {
        prt.print(Printer::Vending, id, 'A');
        _Throw Free();
    }

    // Debit the student's WATCard and dispense soda
    card.withdraw(sodaCost);
    curr_inventory[flavour]--;
    prt.print(Printer::Vending, id, 'B', flavour, curr_inventory[flavour]);
}

void VendingMachine::main() {
    try{
        for (;;) {
        _Accept(~VendingMachine) {
            break;
        }  or _Accept(buy) {}
        or _Accept(inventory) {
        } or _Accept(restocked) {
        }
    }
    } catch ( uMutexFailure::RendezvousFailure &) {   
    }
}

// - sell soda to students at some cost.
// - DONE -- Each vending machine is passed an id in the range [0, NumVendingMachines) for identification, 
// and the MSRP price for a bottle of soda. 


// - DONE - A new vending machine is empty (no stock) and begins by registering with the name server.


// - student calls buy to obtain one of their favourite sodas. 

// - vending machine first checks if the student has sufficient funds to purchase the soda a
// - vending machine second checks if specified soda is available


// - then the student’s WATCard is debited by the cost of a soda. -- note that we will have to check if the watcard exists
//     - raise exceptions Funds or Stock, respectively. 
// - Once a purchase is possible, there is a 1 in 5 chance the soda is free, which is indicated by raising exception Free

// - Periodically, the truck comes by to restock the vending machines with new soda from the bottlig plant
// - Restocking has two steps:
//     - calls inventory to return a pointer to an array containing the amount of each kind of soda currently in the vending machine.
//     - truck uses this information to transfer as much of its
// stock as fits into the machine; for each kind of soda, no more than MaxStockPerFlavour per flavour can be added
// to a machine.
//     - After transferring new soda into the machine by directly modifying the array passed from inventory, the truck calls restocked to indicate the operation is complete
// - The vending machine cannot accept buy calls during restocking
// - the cost member returns the cost of purchasing a soda for this machine. 
// - The getId member returns the identification number of the vending machine.
