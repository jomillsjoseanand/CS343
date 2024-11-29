#include "soda.h"

BottlingPlant::BottlingPlant( Printer & prt, NameServer & nameServer, unsigned int numVendingMachines,
    unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
    unsigned int timeBetweenShipments ) prt(prt), nameServer(nameServer), 
    numVendingMachines(numVendingMachines), maxShippedPerFlavour(maxShippedPerFlavour), maxStockPerFlavour(maxStockPerFlavour)
    timeBetweenShipments(timeBetweenShipments) {
    // Print the constructor message
    prt.print(Printer::BottlingPlant, 'S');
    // TODO: remove use of this 
    // - It begins by creating a truck
    Truck truck = Truck(prt, nameServer, this, numVendingMachines, maxStockPerFlavour);
}

void BottlingPlant::main (){

    try {
        // To simulate a production run of soda, the bottling plant yields for TimeBetweenShipments times (not random)
        yeild(TimeBetweenShipments);

        // Generate random quantities for each flavour
        int total = 0;
        for (unsigned int i = 0; i < 4; ++i) {
            inventory[i] = prng(MaxShippedPerFlavour); // Random quantity for each flavour
            total += inventory[i];
        }
        prt.print(Printer::BottlingPlant, 'G', total);

        //  pickup the production run, pick up just means the shipment is copied into the cargo array passed by the truck.
        _Accept(~BottlingPlant) {
            // Shutdown signal detected
            prt.print(Printer::BottlingPlant, 'F');
            throw Shutdown();
        }
        or _Accept(getShipment) {} 
    } catch (Shutdown &) {
        // Propagate the exception to the correct task
        _Resume Shutdown() _At truck;
    }
}

void BottlingPlant::getShipment( unsigned int cargo[] ){
    for (unsigned int i = 0; i < 4; ++i) {
        cargo[i] = inventory[i];
    }
    prt.print(Printer::BottlingPlant, 'P');
}
