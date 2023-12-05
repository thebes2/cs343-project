#ifndef _BOTTLING_PLANT_H_
#define _BOTTLING_PLANT_H_

#include "printer.h"
#include "nameServer.h"

_Task BottlingPlant {
    Printer &printer;
    NameServer &nameServer;
    unsigned int numVendingMachines, maxShippedPerFlavour, maxStockPerFlavour, timeBetweenShipments, randomQuantity, numFlavours;
    unsigned int *loadingCargo; // temporary variable used for user calling getShipment to drop reference to their cargo array
    uCondition bench; // uCondition bench to make caller of public member wait while main does work
    int currentStock[4] = {}; // current stock of Bottling plant after production run
    void main();
  public:
    enum Flavours {Blues_Black_Cherry=0, Classic_Cream_Soda=1, Rock_Root_Beer=2, Jazz_Lime=3}; // flavours of soda (YOU DEFINE)
    _Event Shutdown {}; // shutdown plant
    BottlingPlant( Printer & prt, NameServer & nameServer, unsigned int numVendingMachines, unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour, unsigned int timeBetweenShipments );
    void getShipment( unsigned int cargo[] );
};

#endif
