#include "soda.h"

BottlingPlant::BottlingPlant( Printer & prt, NameServer & nameServer, unsigned int numVendingMachines, unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour, unsigned int timeBetweenShipments ) : prt(prt), nameServer(nameServer), 
    numVendingMachines(numVendingMachines), maxShippedPerFlavour(maxShippedPerFlavour), maxStockPerFlavour(maxStockPerFlavour), timeBetweenShipments(timeBetweenShipments) {
    
    // Print the constructor message
    
    // TODO: remove use of this 
    //- It begins by creating a truck
    truck = new Truck(prt, nameServer, *this, numVendingMachines, maxStockPerFlavour);
    prt.print(Printer::Kind::BottlingPlant, 'S');

}

void BottlingPlant::main (){

    try {
        // To simulate a production run of soda, the bottling plant yields for TimeBetweenShipments times (not random)
        yield(timeBetweenShipments);

        // Generate random quantities for each flavour
        int total = 0;
        for (unsigned int i = 0; i < 4; ++i) {
            inventory[i] = prng(maxShippedPerFlavour); // Random quantity for each flavour
            total += inventory[i];
        }
        prt.print(Printer::Kind::BottlingPlant, 'G', total);

        //  pickup the production run, pick up just means the shipment is copied into the cargo array passed by the truck.
        _Accept(~BottlingPlant) {
            // Shutdown signal detected
            prt.print(Printer::Kind::BottlingPlant, 'F');
            throw Shutdown();
        }
        or _Accept(getShipment) {} 
    } catch (Shutdown &) {
        // Propagate the exception to the correct task
        _Resume Shutdown() _At *truck;
    }
}

void BottlingPlant::getShipment( unsigned int cargo[] ){
    for (unsigned int i = 0; i < 4; ++i) {
        cargo[i] = inventory[i];
    }
    prt.print(Printer::Kind::BottlingPlant, 'P');
}

BottlingPlant::~BottlingPlant(){
    delete truck;
}