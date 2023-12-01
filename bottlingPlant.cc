#include "bottlingPlant.h"
#include "truck.h"
#include "uPRNG.h"

void BottlingPlant::getShipment(unsigned int cargo[]) {
    loadingCargo = cargo;
    // sync call, go to sleep
    bench.wait((uintptr_t)(void*)&uThisTask());
}

BottlingPlant::BottlingPlant(Printer & prt, NameServer & nameServer, unsigned int numVendingMachines, unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour, unsigned int timeBetweenShipments) :
printer{prt}, nameServer{nameServer}, numVendingMachines{numVendingMachines}, maxShippedPerFlavour{maxShippedPerFlavour}, maxStockPerFlavour{maxStockPerFlavour}, timeBetweenShipments{timeBetweenShipments}, 
randomQuantity{0}, numFlavours{4}, isShutdown{false} {}

void BottlingPlant::main() {
    printer.print(Printer::Kind::BottlingPlant, 'S');
    // create truck
    Truck driver(printer, nameServer, *this, numVendingMachines, maxStockPerFlavour);
    for(;;) {
        if(randomQuantity==0) {
            // perform production run and yield
            randomQuantity = prng(0, maxShippedPerFlavour+1);
            printer.print(Printer::Kind::BottlingPlant, 'G', randomQuantity);
            yield(timeBetweenShipments);
        }
        _Accept(~BottlingPlant) {
            isShutdown = true;
            while(true) {
                _Accept(getShipment) {
                    _Resume Shutdown{} _At *(Truck*)(void*)bench.front();
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
            printer.print(Printer::Kind::BottlingPlant, 'P');
            bench.signalBlock();
        }
    }
    printer.print(Printer::Kind::BottlingPlant, 'F');
}