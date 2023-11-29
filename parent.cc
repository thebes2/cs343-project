#include "parent.h"

void Parent::main() {
    for (;;) {
        for (int i=0;i<parentalDelay;i++) {
            _When(true) _Accept(~Parent) { return; } // exit
            _Else { yield(1); }
        }
        unsigned int id = prng(numStudents);
        unsigned int amt = prng(2) + 1;
        bank.deposit(id, amt);
    }
}

Parent::Parent(Printer & prt, Bank & bank, unsigned int numStudents, unsigned int parentalDelay)
    : printer(prt), bank(bank), numStudents(numStudents), parentalDelay(parentalDelay) {}