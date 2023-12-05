#include "vendingMachine.h"
#include "watCard.h"
#include "student.h"
using namespace std;

VendingMachine::VendingMachine(Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost) :
printer{prt}, nameServer{nameServer}, id{id}, sodaCost{sodaCost}, restocking{false} {}

// public mutex member to get cost of soda
unsigned int VendingMachine::cost() const {
    return sodaCost;
}

// public mutex member to get id of vending machine
unsigned int VendingMachine::getId() const {
    return id;
}

// public mutex member to indicate vending machine has been restocked
void VendingMachine::restocked() {
    // client has to do nothing here
}

// public mutex member to get inventory of vending machine for restocking
unsigned int * VendingMachine::inventory() {
    return sodaInventory;
}

// public mutex member called by students to buy their favourite soda
void VendingMachine::buy(BottlingPlant::Flavours flavour, WATCard & card) {
    currFlavour = flavour; currCard = &card; // save information on stack of vending machine
    bench.wait((uintptr_t)(void*)&uThisTask()); // go to sleep on bench with address in case an exception needs to be resumed
}

void VendingMachine::main() {
    printer.print(Printer::Kind::Vending, id, 'S', sodaCost);
    nameServer.VMregister(this); // register with name server
    for(;;) {
        _Accept(~VendingMachine) {
            break;  
        } or _Accept(inventory) { // prioritize calls by truck to restock machine over students wanting to buy
            printer.print(Printer::Kind::Vending, id, 'r');
            restocking = true;
        } or _When(!restocking) _Accept(buy) { // if machine isn't being currently restock, allow students to buy
            if(currCard->getBalance()<sodaCost) { // if not enough money, Resume funds exception at student asleep on bench
                _Resume Funds{} _At *(Student*)(void*)bench.front();
                bench.signalBlock(); // signal block allows student to return
            }
            else if(sodaInventory[currFlavour]==0) { // if flavour out of stock, Resume stock exception at student 
                _Resume Stock{} _At *(Student*)(void*)bench.front();
                bench.signalBlock(); // signal block to allow student to return
            } 
            else if(prng(1,5)==1) { // 1 in 5 chance soda is free, Resume free exception at student
                _Resume Free{} _At *(Student*)(void*)bench.front();
                bench.signalBlock(); // signal block to allow student to return
                printer.print(Printer::Kind::Vending, id, 'A');
            }
            else { // no exception to throw at student, allow purchase to go through
                currCard->withdraw(sodaCost);
                sodaInventory[currFlavour] --;
                bench.signalBlock(); // wake student up and allow them to return from buy member by momentarily blocking ourselves
                printer.print(Printer::Kind::Vending, id, 'B', currFlavour, sodaInventory[currFlavour]);
            }
        } or _Accept(restocked) { // restocking is done, reset restocking flag to allow purchases to vending machine
            printer.print(Printer::Kind::Vending, id, 'R');
            restocking = false;
        }
    }
    printer.print(Printer::Kind::Vending, id, 'F');
}

