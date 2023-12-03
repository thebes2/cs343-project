#include "parent.h"

void Parent::main() {
    printer.print(Printer::Kind::Parent, 'S');
    for (;;) {
        for (unsigned int i=0;i<parentalDelay;i++) {
            _When(true) _Accept(~Parent) { goto exit; }
            _Else { yield(1); }
        }
        unsigned int id = prng(numStudents);
        unsigned int amt = prng(3) + 1;
        printer.print(Printer::Kind::Parent, 'D', id, amt);
        bank.deposit(id, amt);
    } exit:
    printer.print(Printer::Kind::Parent, 'F');
}

Parent::Parent(Printer & prt, Bank & bank, unsigned int numStudents, unsigned int parentalDelay)
    : printer(prt), bank(bank), numStudents(numStudents), parentalDelay(parentalDelay) {}