#include <iostream>
#include <vector>
#include <string>

#include "soda.h"

// Flush function to print the current buffered states of all voters
void Printer::flush() {
    bool print = false; // Flag to determine if there are any buffered states to print

    // Check if there's anything to print by scanning each voter's buffer
    for (unsigned int i = 0; i < buffer.size(); i++) {
        if (!buffer[i].empty) {
            print = true;  // Set flag to true if there is at least one non-empty buffer
            break;
        }
    }

    if (!print) return; // If no data to print, exit function

    // Print the buffered states of all voters
    for (unsigned int i = 0; i < numColumns; i++) {
        if (i > 0) cout << '\t';  // Print tab space between voters
        
        if (!buffer[i].empty) {  // If voter data is not empty, print it
            std::cout << buffer[i].state;
            if (buffer[i].value1 != -1) std::cout << buffer[i].value1;
            if (buffer[i].value2 != -1) std::cout << "," << buffer[i].value2;
        }
    }
    std::cout << std::endl;
    
    // Clear the buffer
    for (auto& entry : buffer) {
        entry.empty = true;
        entry.value1 = entry.value2 = -1;
    }
}

// Printer constructor to initialize the number of voters and their state buffer
Printer::Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers ) numStudents(numStudents), numVendingMachines(numVendingMachines), numCouriers(numCouriers) {
    numColumns = 6 + numStudents + numVendingMachines + numCouriers; // Fixed + variable instances
    buffer.resize(numColumns);

    // Print column headers
    const char* headers[] = { "Parent", "Groupoff", "WATOff", "Names", "Truck", "Plant" };
    for (unsigned int i = 0; i < numColumns; i++) {
        if (i < 6) {
            std::cout << headers[i];
        } else if (i < 6 + numStudents) {
            std::cout << "Stud" << (i - 6);
        } else if (i < 6 + numStudents + numVendingMachines) {
            std::cout << "Mach" << (i - 6 - numStudents);
        } else {
            std::cout << "Cour" << (i - 6 - numStudents - numVendingMachines);
        }
        if (i != numColumns - 1) std::cout << '\t';
    }
    std::cout << std::endl;

    // Print separator row
    for (unsigned int i = 0; i < numColumns; i++) {
        if (i != 0) std::cout << '\t';
        std::cout << "*******";
    }
    std::cout << std::endl;
}

Printer::~Printer(){
    flush();
    std::cout << "*****************" << std::endl;
}

// Determine the column index for a given kind and lid
unsigned int Printer::determineColumn(Kind kind, unsigned int lid = 0) const {
    switch (kind) {
        case Parent: return 0;
        case Groupoff: return 1;
        case WATCardOffice: return 2;
        case NameServer: return 3;
        case Truck: return 4;
        case BottlingPlant: return 5;
        case Student: return 6 + lid;
        case Vending: return 6 + numStudents + lid;
        case Courier: return 6 + numStudents + numVendingMachines + lid;
        default: throw std::invalid_argument("Invalid kind");
    }
}

void Printer::print( Kind kind, char state ){
    // detemine which col
    unsigned int col = determineColumn(kind);
    if (!buffer[col].empty) flush();
    buffer[col] = { state, -1, -1, false };
}

// Vending machine and machine id 
// Finished by parent int val total gifts given
// registering new vending machine by nameserver 
void Printer::print( Kind kind, char state, unsigned int value1 ){
    unsigned int col = determineColumn(kind);
    if (!buffer[col].empty) flush();
    buffer[col] = { state, (int)value1, -1, false };
}

// Print state with two additional values
void Printer::print( Kind kind, char state, unsigned int value1, unsigned int value2 ){
    unsigned int col = determineColumn(kind);
    if (!buffer[col].empty) flush();
    buffer[col] = { state, (int)value1, (int)value2, false };
}

void Printer::print( Kind kind, unsigned int lid, char state ){
    unsigned int col = determineColumn(kind, lid);
    if (!buffer[col].empty) flush();
    buffer[col] = { state, -1, -1, false };
}

void Printer::print( Kind kind, unsigned int lid, char state, unsigned int value1 ){
    unsigned int col = determineColumn(kind, lid);
    if (!buffer[col].empty) flush();
    buffer[col] = { state, (int)value1, -1, false };
}

void Printer::print( Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2 ){
    unsigned int col = determineColumn(kind, lid);
    if (!buffer[col].empty) flush();
    buffer[col] = { state, (int)value1, (int)value2, false };
}


