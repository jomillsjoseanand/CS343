#ifndef __BOTTLING_PLANT_H__
#define __BOTTLING_PLANT_H__

#include "truck.h"

_Task BottlingPlant {
	void main();
    Printer & prt;
    NameServer & nameServer;
    unsigned int numVendingMachines;
	unsigned int maxShippedPerFlavour;
    unsigned int maxStockPerFlavour;
	unsigned int timeBetweenShipments;
    unsigned int inventory[4];
    Truck truck;
  
  public:
	enum Flavours { Blues_BlackCherry, Classic_CreamSoda, Rock_RootBeer, Jazz_Lime  };	// flavours of soda (YOU DEFINE)
	
    _Exception Shutdown {};					// shutdown plant
	
    BottlingPlant( Printer & prt, NameServer & nameServer, unsigned int numVendingMachines,
				 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
				 unsigned int timeBetweenShipments );
	void getShipment( unsigned int cargo[] );
};


#endif