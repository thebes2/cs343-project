#ifndef _VENDING_MACHINE_H
#define _VENDING_MACHINE_H
#include "watCard.h"
#include "bottlingPlant.h"


_Task VendingMachine {
    Printer &printer;
    NameServer &nameserver;
    unsigned int id, sodaCost;
    int sodaInventory[4] = {};
    bool restocking;
    void main();
  public:
    _Event Free {}; // free, advertisement
    _Event Funds {}; // insufficient funds
    _Event Stock {}; // flavour out of stock
    VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost );
    void buy( BottlingPlant::Flavours flavour, WATCard & card );
    unsigned int * inventory() __attribute__(( warn_unused_result ));
    void restocked();
    _Nomutex unsigned int cost() const;
    _Nomutex unsigned int getId() const;
};