#ifndef _TRUCK_H_
#define _TRUCK_H_

#include "printer.h"
#include "nameServer.h"
#include "bottlingPlant.h"

_Task Truck {
    Printer &printer;
    NameServer &nameServer;
    BottlingPlant & plant;
    unsigned int numVendingMachines, maxStockPerFlavour;
    unsigned int *shipment;
    void main();
    bool empty(unsigned int*);
  public:
    Truck(Printer & prt, NameServer & nameServer, BottlingPlant & plant, unsigned int numVendingMachines, unsigned int maxStockPerFlavour);
};
#endif