#include "bottlingPlant.h"
#include "uPRNG.h"

void BottlingPlant::getShipment(unsigned int cargo[]) {
    loadingCargo = cargo;
    // sync call, go to sleep
    bench.wait(*this);
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
                _Accept(getShipment) {
                    _Resume Shutdown{} _At bench.front();
                    bench.signalBlock();
                }
                _Else {break;}
            }
            break;
        } or _Accept(getShipment) {
            for(unsigned int i=0; i<numFlavours; i++) {
                loadingCargo[i] = randomQuantity;
            }
            randomQuantity = 0;
            bench.signalBlock();
        }
    }
}