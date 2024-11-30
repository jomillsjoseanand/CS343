#include "soda.h"

BottlingPlant::BottlingPlant( Printer & prt, NameServer & nameServer, unsigned int numVendingMachines, unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour, unsigned int timeBetweenShipments ) : prt(prt), nameServer(nameServer), 
    numVendingMachines(numVendingMachines), maxShippedPerFlavour(maxShippedPerFlavour), maxStockPerFlavour(maxStockPerFlavour), timeBetweenShipments(timeBetweenShipments) {
    prt.print(Printer::Kind::BottlingPlant, 'S');
    //- It begins by creating a truck
    truck = new Truck(prt, nameServer, *this, numVendingMachines, maxStockPerFlavour);
    should_throw = false;
}

void BottlingPlant::main (){

        for (;;) {
            // To simulate a production run of soda, the bottling plant yields for TimeBetweenShipments times (not random)
            yield(timeBetweenShipments);

            // Generate random quantities for each flavour
            int total = 0;
            for (unsigned int i = 0; i < 4; ++i) {
                inventory[i] = prng(0, maxShippedPerFlavour); // Random quantity for each flavour
                total += inventory[i];
            }
            prt.print(Printer::Kind::BottlingPlant, 'G', total);

            //  pickup the production run, pick up just means the shipment is copied into the cargo array passed by the truck.
            _Accept(~BottlingPlant) {
                // Shutdown signal detected
                break;
            } or _Accept(getShipment) {}
            
        }

        // accept last getshipment call
        should_throw = true;
        try {
            _Accept(getShipment) {}
        } catch (uMutexFailure::RendezvousFailure& e) {
        }

        prt.print(Printer::Kind::BottlingPlant, 'F');
    
}

void BottlingPlant::getShipment( unsigned int cargo[] ){

    // For the last shipment throw Shutdown exception 
    if (should_throw) {
        _Throw Shutdown();
    }

    for (unsigned int i = 0; i < 4; ++i) {
        cargo[i] = inventory[i];
    }

    prt.print(Printer::Kind::BottlingPlant, 'P');
}

BottlingPlant::~BottlingPlant(){
    delete truck;
}