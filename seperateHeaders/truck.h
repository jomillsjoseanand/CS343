#ifndef __TRUCK_H__
#define __TRUCK_H__

_Task Truck {
	void main();
    Printer & prt;
    NameServer & nameServer;
    BottlingPlant & plant;
	unsigned int numVendingMachines;
    unsigned int maxStockPerFlavour;
    unsigned int cargo[4];


  public:
	Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant,
		   unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
};



#endif