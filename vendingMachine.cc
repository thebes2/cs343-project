#include "vendingMachine.h"
#include "watCard.h"
using namespace std;

VendingMachine::VendingMachine(Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost) :
printer{prt}, nameServer{nameServer}, id{id}, sodaCost{sodaCost}, restocking{false} {}

unsigned int cost() const {
    return sodaCost;
}

unsigned int getId() const {
    return Id;
}

void VendingMachine::restocked() {
    // reset flag variable
}

unsigned int * VendingMachine::inventory() {
    return &sodaInventory;
}

void VendingMachine::buy(BottlingPlant::Flavours flavour, WATCard & card) {
    currFlavour = flavour;
    currCard = card;
    bench.wait(*this);
}


void VendingMachine::main() {
    // register with name server
    nameServer.VMRegister(*this);
    for(;;) {
        _Accept(~VendingMachine) {
            break;
        } or _When(!restocking) _Accept(buy) {
            // buy done by student or exception was thrown
            if(currCard.getBalance()<sodaCost) {
                _Resume Funds{} _At bench.front();
            } else if(sodaInventory[flavour]==0) {
                _Resume Stock{} _At bench.front();
            } else if(prng(5)==0) {
                _Resume Free{} _At bench.front();
            } else {
                card.withdraw(sodaCost);
            }
            bench.signalBlock();
        } or _Accept(inventory) {
            restocking = true;
        } or _Accept(restocked) {
            restocking = false;
        }
    }
}

