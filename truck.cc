#include "soda.h"

Truck::Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant, unsigned int numVendingMachines, unsigned int maxStockPerFlavour ) : prt(prt), nameServer(nameServer), plant(plant), 
           numVendingMachines(numVendingMachines), maxStockPerFlavour(maxStockPerFlavour)  {}


void Truck::main(){
    prt.print(Printer::Kind::Truck, 'S');

    // Obtain vending machine locations from the NameServer
    VendingMachine **vendingMachines = nameServer.getMachineList();

    unsigned int startMachineIdx = 0; // Track the current vending machine for cyclic restocking

    try {
        _Enable{
            for (;;) {

                // Simulate a coffee break by yielding randomly between 1 and 10 times
                yield(prng(1, 10));
                

                // AFTER shipped -- truck then calls BottlingPlant::getShipment to 
                // obtain a new shipment of soda; any soda still on the 
                // truck is thrown away as it is past its 
                // due date.
                plant.getShipment(cargo);

                // To ensure fairness, the vending machines are restocked in cyclic order starting at
                // the vending machine after the last machine the truck restocked, until there is no more soda on the truck or the
                // truck has made a complete cycle of all the vending machines; so there is no guarantee each vending machine
                // is completely restocked or the entire complement of vending machines is restocked or all the soda on the truck
                // is used. 
                unsigned int currentMachineIdx = startMachineIdx ;
                unsigned int totalSoda = 0;
                for (unsigned int amount : cargo) totalSoda += amount;
                prt.print(Printer::Kind::Truck, 'P', totalSoda);

                do {
                    
                    // the vending machine after the last machine the truck restocked, until there is no more soda on the truck or the
                    // truck has made a complete cycle
                    if (totalSoda == 0) break;

                    prt.print(Printer::Kind::Truck, 'd', currentMachineIdx, totalSoda);

                    VendingMachine* curr_machine = vendingMachines[currentMachineIdx];
                    int bottlesNotReplenished = 0;
                    unsigned int * inventory = curr_machine->inventory();
                    for (unsigned int i = 0; i < 4; ++i) {
                        unsigned int spaceAvailable = maxStockPerFlavour - inventory[i];
                        unsigned int stockToAdd = std::min(spaceAvailable, cargo[i]);
                        if (spaceAvailable > cargo[i]) {
                                bottlesNotReplenished += spaceAvailable - cargo[i];
                        }
                        cargo[i] -= stockToAdd;
                        inventory[i] += stockToAdd;
                        totalSoda -= stockToAdd;
                    }

                    // Update the vending machine inventory
                    curr_machine->restocked();

                    // TODO: Print unstocked soda
                    if (bottlesNotReplenished > 0) {
                        prt.print(Printer::Kind::Truck, 'U', currentMachineIdx, bottlesNotReplenished);
                    }

                    prt.print(Printer::Kind::Truck, 'D', currentMachineIdx, totalSoda);
                    
                    // Move to the next vending machine cyclically
                    currentMachineIdx = (currentMachineIdx + 1) % numVendingMachines;

                    // 1 in 100 chance of a flat tire; takes 10 yields to fix
                    if (prng(1, 100) == 1) {
                        prt.print(Printer::Kind::Truck, 'W');
                        for (int i = 0; i < 10; i++) yield();
                    }
                
                } while (currentMachineIdx != startMachineIdx);

                // update last vending machine we left off at
                startMachineIdx = currentMachineIdx;
                
            }
        }
        
    }
    // If the bottling plant is closing down, the truck terminates.
    _CatchResume (BottlingPlant::Shutdown &) {
        throw BottlingPlant::Shutdown();
    }
    
    catch (BottlingPlant::Shutdown &) {
        // Handle plant shutdown
        prt.print(Printer::Kind::Truck, 'F');
    }
}
