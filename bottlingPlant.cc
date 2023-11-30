#include "bottlingPlant.h"
#include "uPRNG.h"

BottlingPlant::getShipment(unsigned int cargo[]) {
    if(isShutdown) {
        _Throw Shutdown{};
    }
    for(unsigned int i=0; i<numFlavours; i++) {
        cargo[i] = randomQuantity;
    }
}

BottlingPlant::BottlingPlant(Printer & prt, NameServer & nameServer, unsigned int numVendingMachines, unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour, unsigned int timeBetweenShipments) :
printer{prt}, nameServer{nameServer}, numVendingMachines{numVendingMachines}, maxShippedPerFlavour{maxShippedPerFlavour}, maxStockPerFlavour{maxStockPerFlavour}, timeBetweenShipments{timeBetweenShipments}, 
randomQuantity{0}, numFlavours{4}, isShutdown{false} {}

void BottlingPlant::main() {
    // create truck
    Truck driver(printer, nameServer, *this, numVendingMachines, maxStockPerFlavour);
    for(;;) {
        if(randomQuantity==0) {
            // perform production run and yield
            randomQuantity = prng(0, maxShippedPerFlavour+1);
            yield(timeBetweenShipments);
        }
        _Accept(~BottlingPlant) {
            isShutdown = true;
            while(true) {
                try {
                    _Accept(getShipment) {}
                    _Else {break;}
                } catch (uMutexFailure::RendezvousFailure &) {
                     // likely happened because getShipment couldn't be accepted since bottling plant is shutting down
                }
            }
            break;
        } or _Accept(getShipment) {
            randomQuantity = 0;
        }
    }
}