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
    if(card.getBalance()>=sodaCost) {
        if(sodaInventory[flavour]>0)> {
            if(yield(5)==0) {
                uRendezvousAcceptor();
                _Throw Free{};
            } else  {
                // what happens if they now suddenly don't have money
                card.withdraw(sodaCost);
            }
        } else {
            uRendezvousAcceptor();
            _Throw Stock{};
        }
     } else {
        uRendezvousAcceptor();
        _Throw Funds{};
    }
}

void VendingMachine::main() {
    // register with name server
    nameServer.VMRegister(*this);
    for(;;) {
        _Accept(~VendingMachine) {
            break;
        } or _When(!restocking) _Accept(buy) {

        } or _Accept(inventory) {
            restocking = true;
        } or _Accept(restocked) {
            restocking = false;
        }
    }
}

