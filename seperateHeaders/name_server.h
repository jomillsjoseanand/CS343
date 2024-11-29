
#ifndef __NAME_SERVER_H__
#define __NAME_SERVER_H__

_Task NameServer {
	void main();
    Printer & prt;
    unsigned int numVendingMachines;
    unsigned int numStudents;
	VendingMachine ** vendingMachines;
    unsigned int * studentMachineIndex;
    uCondition bench; 

  public:
	NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents );
	~NameServer();
    void VMregister( VendingMachine * vendingmachine );
	VendingMachine * getMachine( unsigned int id ) __attribute__(( warn_unused_result ));
	VendingMachine ** getMachineList() __attribute__(( warn_unused_result ));
};

// - NameServer is a server task used to manage the vending-machine names.

// - DONE - The name server is passed the number of vending machines, NumVendingMachines, and the number of students, NumStudents

// - logically distributes students evenly across the vending machines using round-robin 
//     - student id 0 is assigned to the first registered vending-machine, student id 1 is assigned to the second registered vendingmachine, etc.,

// - DONE -- Vending machines call VMregister to register themselves so students can subsequently locate them. 

// - getMachine to find a vending machine, and the name server must cycle through the vending machines separately for each student 
// starting from their initial position
// via modulo incrementing to ensure a student has a chance to visit every machine. T
// - DONE -- truck calls getMachineList To obtain an array of pointers to vending machines so it can visit each machine to deliver new soda. 
// - vending machines are registered before accepting calls to other members.

#endif