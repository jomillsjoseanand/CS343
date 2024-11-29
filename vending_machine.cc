#include "soda.h"

VendingMachine::VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost ) prt(prt), nameServer(nameServer), id(id), sodaCost(cost), restocking(false){
    inventory = new unsigned int[4];
    for (int i = 0; i < 4; i++) {
        inventory[i] = 0;
    }

    // TODO: remove this 
    nameServer.VMregister(this);
    prt.print(Printer::Vending, id, 'S', sodaCost);

}

_Nomutex unsigned int VendingMachine::cost() const {
    return sodaCost;
}

_Nomutex unsigned int VendingMachine::getId() const {
    return id;
}

unsigned int *VendingMachine::inventory() {
    restocking = true;
    return inventory;
}

void VendingMachine::restocked() {
    restocking = false;
    prt.print(Printer::Vending, id, 'R');
}



void VendingMachine::buy( BottlingPlant::Flavours flavour, WATCard &card ) {
    if (restocking) return; // Ignore buy calls during restocking

    // vending machine first checks if the student has sufficient funds to purchase the soda 
    if (card.getBalance() < sodaCost) _Resume Funds() _At resumer(); // Check if the student has enough funds
    if (inventory[flavour] == 0) _Resume Stock() _At resumer();; // Check if the soda is in stock
    
    // 1 in 5 chance the soda is free
    if (mprng(4) == 0) {
        _Resume Free() _At resumer();
    }

    // Debit the student's WATCard and dispense soda
    card.withdraw(sodaCost);
    inventory[flavour]--;
    prt.print(Printer::Vending, id, 'B', flavour, inventory[flavour]);
}

void VendingMachine::main() {
    prt.print(Printer::Vending, id, 'S', sodaCost);
    for (;;) {
        _Accept(~VendingMachine) {
            break;
        } or _Accept(inventory) {
            prt.print(Printer::Vending, id, 'r');
        } or _Accept(restocked) {
            prt.print(Printer::Vending, id, 'R');
        } or _When(!restocking) _Accept(buy) {
            prt.print(Printer::Vending, id, 'B', flavour, inventory[flavour]);
        } 
    }

    prt.print(Printer::Vending, id, 'F');
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