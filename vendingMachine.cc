#include "vendingMachine.h"
#include "watCard.h"
#include "student.h"
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
    nameServer.VMregister(id);
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
                _Resume Free{} _At *(Student*)(void*)bench.front();
            } else {
                currCard->withdraw(sodaCost);
            }
            bench.signalBlock();
        } or _Accept(inventory) {
            restocking = true;
        } or _Accept(restocked) {
            restocking = false;
        }
    }
}

