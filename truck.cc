#include "soda.h"

Truck::Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant,
		   unsigned int numVendingMachines, unsigned int maxStockPerFlavour ) prt(prt), nameServer(nameServer), plant(plant), 
           numVendingMachines(numVendingMachines), maxStockPerFlavour(maxStockPerFlavour)  {}


void Truck::main(){

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
                
                do {
                    
                    // the vending machine after the last machine the truck restocked, until there is no more soda on the truck or the
                    // truck has made a complete cycle
                    if (totalSoda == 0) break;

                    VendingMachine* curr_machine = vendingMachines[currentMachineIdx];
                    for (unsigned int i = 0; i < BottlingPlant::NumFlavours; ++i) {
                        unsingned int spaceAvailable = maxStockPerFlavour - curr_machine.inventory()[i];
                        unsigned int stockToAdd = std::min(spaceAvailable, cargo[i]);
                        cargo[i] -= stockToAdd;
                        curr_machine.inventory()[i] += stockToAdd;
                        totalSoda -= stockToAdd;
                    }

                    // Update the vending machine inventory
                    curr_machine->restocked();

                    // TODO: Print restocking results
                    
                    // Move to the next vending machine cyclically
                    currentMachineIdx = (currentMachineIdx + 1) % numVendingMachines;

                    // 1 in 100 chance of a flat tire; takes 10 yields to fix
                    if (prng(1, 100) == 1) {
                        // prt.print(Printer::Kind::Truck, "Flat tire! Fixing...");
                        for (int i = 0; i < 10; i++) yield();
                        // prt.print(Printer::Kind::Truck, "Flat tire fixed.");
                    }
                
                } while (currentMachineIdx != startMachineIdx);

                // update last vending machine we left off at
                startMachineIdx = currentMachineIdx;
                
            }
        }
        
    }
    // If the bottling plant is closing down, the truck terminates.
    _CatchResume (BottlingPlant::Shutdown &) {
        throw Shutdown();
    }
    
    catch (BottlingPlant::Shutdown &) {
        // Handle plant shutdown
        prt.print(Printer::Kind::Truck, "F");
    }
}
