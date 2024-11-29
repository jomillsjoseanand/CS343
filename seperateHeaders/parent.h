#ifndef __PARENT_H__
#define __PARENT_H__

#include "bank.h"
#include "printer.h"
#include "uPRNG.h"                  // For pseudorandom number generation

_Task Parent {

    void main();
    Printer& prt;
    Bank& bank;
    unsigned int numStudents;
    unsigned int parentalDelay;
    
    public:
        Parent( Printer & prt, Bank & bank, unsigned int numStudents, unsigned int parentalDelay );
};

#endif