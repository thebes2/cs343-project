#include "vendingMachine.h"
#include "watCard.h"
#include "student.h"
#include <iostream> // FOR DEBUGGING PURPOSES ONLY
using namespace std;

VendingMachine::VendingMachine(Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost) :
printer{prt}, nameServer{nameServer}, id{id}, sodaCost{sodaCost}, restocking{false} {}

unsigned int VendingMachine::cost() const {
    return sodaCost;
}

unsigned int VendingMachine::getId() const {
    return id;
}

void VendingMachine::restocked() {
    // reset flag variable
}

unsigned int * VendingMachine::inventory() {
    return sodaInventory;
}

void VendingMachine::buy(BottlingPlant::Flavours flavour, WATCard & card) {
    currFlavour = flavour;
    currCard = &card;
    bench.wait((uintptr_t)(void*)this);
}

void VendingMachine::main() {
    // register with name server
    printer.print(Printer::Kind::Vending, id, 'S', sodaCost);
    nameServer.VMregister(this);
    cout << "REGISTERED" << endl;
    for(;;) {
        _Accept(~VendingMachine) {
            break;
        } or _When(!restocking) _Accept(buy) {
            // buy done by student or exception was thrown
            if(currCard->getBalance()<sodaCost) {
                _Resume Funds{} _At *(Student*)(void*)bench.front();
            } else if(sodaInventory[currFlavour]==0) {
                _Resume Stock{} _At *(Student*)(void*)bench.front();
            } else if(prng(5)==0) {
                printer.print(Printer::Kind::Vending, 'A');
                _Resume Free{} _At *(Student*)(void*)bench.front();
            } else {
                currCard->withdraw(sodaCost);
                sodaInventory[currFlavour] --;
                printer.print(Printer::Kind::Vending, id, 'B', currFlavour, sodaInventory[currFlavour]);
            }
            bench.signalBlock();
        } or _Accept(inventory) {
            printer.print(Printer::Kind::Vending, id, 'r');
            restocking = true;
        } or _Accept(restocked) {
            printer.print(Printer::Kind::Vending, id, 'R');
            restocking = false;
        }
    }
    printer.print(Printer::Kind::Vending, id, 'F');
}

