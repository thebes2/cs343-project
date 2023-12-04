#include "parent.h"

void Parent::main() {
    printer.print(Printer::Kind::Parent, 'S');
    for (;;) {
        _Accept(~Parent) {
            break;
        }
        _Else {
            yield(parentalDelay);
            unsigned int id = prng(0, numStudents-1);
            unsigned int amt = prng(1, 3);
            printer.print(Printer::Kind::Parent, 'D', id, amt);
            bank.deposit(id, amt);
        }
    } 
    printer.print(Printer::Kind::Parent, 'F');
}

Parent::Parent(Printer & prt, Bank & bank, unsigned int numStudents, unsigned int parentalDelay)
    : printer(prt), bank(bank), numStudents(numStudents), parentalDelay(parentalDelay) {}