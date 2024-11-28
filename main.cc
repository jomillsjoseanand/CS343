#include <uPRNG.h>

#include <iostream>

#include "printer.h"
#include "config.h"

#include "vending_machine.h"
#include "bottling_plant.h"
#include "bank.h"
#include "name_server.h"
#include "parent.h"
#include "watcard_office.h"
#include "groupoff.h"
#include "students.h"


using namespace std;

int main(int argc, char* argv[]) {
    struct cmd_error {};

    // Initialize default values for command-line arguments if not provided.
    string configFile = "soda.config";
    unsigned int seed = 0; // 0 means use PRNG default
    unsigned int processors = 1;

    // Argument parsing
    try {
        if (argc > 1 && strcmp(argv[1], "d") != 0) {
            configFile = argv[1];
        }
        if (argc > 2 && strcmp(argv[2], "d") != 0) {
            seed = stoi(argv[2]);
            if (seed <= 0) throw cmd_error();
        }
        if (argc > 3 && strcmp(argv[3], "d") != 0) {
            processors = stoi(argv[3]);
            if (processors <= 0) throw cmd_error();
        }
        if (argc > 4) {
            throw cmd_error();
        }
    } catch (...) {
        cerr << "Usage: " << argv[0] 
             << " [ config-file | 'd' (default soda.config) "
             << "[ seed (> 0) | 'd' (default random) "
             << "[ processors (> 0) | 'd' (default 1) ] ]"
             << endl;
        exit(EXIT_FAILURE);
    }

    if (seed != 0){
        set_seed(seed);
    }

    // Set up additional processors
    uProcessor p[processors - 1] __attribute__((unused));

    // Read configuration file
    Config config;
    try {
        processConfigFile(configFile, config);
    } catch (const exception &e) {
        cerr << "Error reading configuration file: " << e.what() << endl;
        exit(EXIT_FAILURE);
    }

    // creates in order: printer, bank, parent, WATCard office, groupoff, name server, vending machines, bottling plant, and students. 
    Printer printer(config.numStudents, config.numVendingMachines, config.numCouriers);
    Bank bank(config.numStudents);
    Parent parent(printer, bank, config.numStudents, config.parentalDelay);
    WATCardOffice watCardOffice(printer, bank, config.numCouriers);
    Groupoff groupoff(printer, config.numStudents, config.sodaCost);
    NameServer nameServer(printer, config.numVendingMachines, config.numStudents);
    VendingMachine *vendingMachines[config.numVendingMachines];

    for (unsigned int i = 0; i < config.numVendingMachines; ++i) {
        vendingMachines[i] = new VendingMachine(printer, nameServer, i, config.sodaCost, config.maxStockPerFlavour);
    }

    BottlingPlant *bottlingPlant = new BottlingPlant(
        printer, nameServer, config.numVendingMachines, config.maxShippedPerFlavour,
        config.maxStockPerFlavour, config.timeBetweenShipments);

    Student *students[config.numStudents];
    for (unsigned int i = 0; i < config.numStudents; ++i) {
        students[i] = new Student(printer, nameServer, watCardOffice, groupoff, i, config.maxPurchases);
    }

    // Wait for students to complete
    for (unsigned int i = 0; i < config.numStudents; ++i) {
        delete students[i];
    }

    // Note, there is one trick in closing down the system: delete the 
    // bottling plant before deleting the vending machines to allow the
    // truck to complete its final deliveries to the vending machines; 
    // otherwise, a deadlock can occur.
    delete bottlingPlant;

    // delete vending machine
    for (unsigned int i = 0; i < config.numVendingMachines; ++i) {
        delete vendingMachines[i];
    }

}