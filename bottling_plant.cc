#include "bottling_plant.h"

BottlingPlant::BottlingPlant( Printer & prt, NameServer & nameServer, unsigned int numVendingMachines,
    unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
    unsigned int timeBetweenShipments ) prt(prt), nameServer(nameServer), 
    numVendingMachines(numVendingMachines), maxShippedPerFlavour(maxShippedPerFlavour), maxStockPerFlavour(maxStockPerFlavour)
    timeBetweenShipments(timeBetweenShipments) {

    // TODO: remove use of this 
    // - It begins by creating a truck
    Truck truck = Truck(prt, nameServer, this, numVendingMachines, maxStockPerFlavour);
}

void BottlingPlant::main (){

    try {
        // To simulate a production run of soda, the bottling plant yields for TimeBetweenShipments times (not random)
        yeild(TimeBetweenShipments);


        // Generate random quantities for each flavour
        for (unsigned int i = 0; i < 4; ++i) {
            inventory[i] = prng(MaxShippedPerFlavour); // Random quantity for each flavour
        }

        //  pickup the production run, pick up just means the shipment is copied into the cargo array passed by the truck.
        _Accept(~BottlingPlant) {
            // Shutdown signal detected
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
}