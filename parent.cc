#include "parent.h"

void Parent::main() {
    printer.print(Printer::Kind::Parent, 'S');
    for (;;) {
        _Accept(~Parent) {
            break;
        }
        _Else { // busy waiting and delivering money to student(s)
            yield(parentalDelay);
            unsigned int amt = prng(1, 3);
            unsigned int id = prng(0, numStudents-1);
            bank.deposit(id, amt);
            printer.print(Printer::Kind::Parent, 'D', id, amt);
        }
    } 
    printer.print(Printer::Kind::Parent, 'F');
}

// ctor
Parent::Parent(Printer & prt, Bank & bank, unsigned int numStudents, unsigned int parentalDelay)
    : printer(prt), bank(bank), numStudents(numStudents), parentalDelay(parentalDelay) {}