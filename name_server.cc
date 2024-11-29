#include "soda.h"

NameServer::NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents ) prt(prt), numVendingMachines(numVendingMachines), numStudents(numStudents), vendingMachines(new VendingMachine*[numVendingMachines]), studentPostions(numStudents, 0) {
    // TODO: verify print statement
    prt.print(Printer::Kind::NameServer, 'S'); // Indicate start of NameServer
}

NameServer::~NameServer() {
    prt.print(Printer::Kind::NameServer, 'F'); // Indicate finish of NameServer
}

// Vending machines call VMregister to register themselves so students can subsequently locate them. 
void NameServer::VMregister( VendingMachine * vendingmachine ){
    vendingMachines[vendingmachine.getId()] = vendingmachine;
    prt.print(Printer::Kind::NameServer, 'R', vendingMachine->getId()); // Log registration
}

// - getMachine to find a vending machine, and the name server must cycle through the vending
//  machines separately for each student starting from their initial position
VendingMachine * NameServer::getMachine( unsigned int id ) __attribute__(( warn_unused_result )) {
    bench.wait();
    VendingMachine * machine = vendingMachines[studentPostions[id]];
    studentPostions[id] = (studentPostions[id] + 1) % numVendingMachines;
    prt.print(Printer::Kind::NameServer, 'N', id, machine->getId()); // Log request for machine
    return machine;
}

// return list of registered vending machines
VendingMachine ** VendingMachine::getMachineList() __attribute__(( warn_unused_result )) {
    bench.wait();
    return vendingMachines;
}

// Main function for NameServer
// - All vending machines are registered before accepting calls to other members.
// - logically distributes students evenly across the vending machines using round-robin 
//     - student id 0 is assigned to the first registered vending-machine, student id 1 is assigned to the second registered vendingmachine, etc.,
void NameServer::main() {    
    
    // - All vending machines are registered before accepting calls to other members.
    while (registeredMachines < numVendingMachines) {
        _Accept(VMregister) or _Accept(getMachine) or _Accept(getMachineList);
    }

    // Round robin assignment of students to vending machines
    int curStudentVendingMachineId = 0;
    for (int i = 0; i < numStudents; i++) {
        studentPostions[i] = curStudentVendingMachineId;
        curStudentVendingMachineId = (curStudentVendingMachineId + 1) % numVendingMachines;
    }
    
    for(;;) {
        _Accept(~NameServer) {
            break;
        } or _Accept(getMachine) {
            bench.signalBlock();
        } or _Accept(getMachineList) {
            bench.signalBlock();
        }   
    }
}