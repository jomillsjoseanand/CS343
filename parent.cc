#include "parent.h"

Parent::Parent( Printer & prt, Bank & bank, unsigned int numStudents, unsigned int parentalDelay ) prt(prt), bank(bank), numStudents(numStudents), parentalDelay(parentalDelay) {}

// - Parent task periodically gives a random amount of money [$1, $3] to a random student [0, NumStudents),
// in that order. 
// - Before each gift is transferred (including the first), the parent yields for parentalDelay times (not
// random).
// - The parent must check for a call to its destructor to know when to 
// terminate. 
// - The parent must check for a call to its destructor to know when to terminate. Since it must not block
// on this call, it is necessary to use a terminating _Else on the accept statement. Hence, the parent is performing
// a yielding busy-wait on calls to its destructor.
void Parent::main(){

    printer.print(Printer::Kind::Parent, 'S'); // Parent start message

    while (true) {
        _Accept(~Parent) {
            break; // Destructor called, terminate the loop
        } _Else {
            yield(parentalDelay); // Yield for parentalDelay times

            // Generate a random amount of money between $1 and $3
            int amount = prng(1, 3);

            // Select a random student to receive the money
            int studentId = prng(0, numStudents - 1);

            // Deposit the money into the student's account
            bankRef.deposit(studentId, amount);

            // Print the gift message
            printer.print(Printer::Kind::Parent, 'D', studentId, amount);
        }
    }

    printer.print(Printer::Kind::Parent, 'F'); // Parent finish message
}